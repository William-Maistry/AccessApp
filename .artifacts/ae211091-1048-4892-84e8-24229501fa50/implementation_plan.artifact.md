# Implementation Plan - Multi-Passenger Attendance & Vehicle Verification

This plan details a major update to the attendance tracking system, moving from a single-person scan to a group-based flow with vehicle validation and ownership checks.

## Proposed Navigation Flow

1.  **Action Hub**: Click "Scan In" or "Scan Out".
2.  **QR Scan + Passcode**: Authenticate the **Driver**.
3.  **[NEW] Vehicle Prompt**: Ask "Did you come with a vehicle?".
    - If **No**: Move to Passenger/Finalization step.
    - If **Yes**: Open camera to scan Vehicle Disc.
4.  **[NEW] Vehicle Verification**:
    - Check Expiry (Red alert if expired, Amber if < 30 days).
    - **Ownership Check**: Compare disc `licence_code` with driver's `licence_number`.
    - If mismatch: Query database for the owner's name and store as "Came with {Owner}'s vehicle".
5.  **[NEW] Participant Hub**:
    - Button: **"Add Another Passenger"** -> Loops back to QR Scan + Passcode for the passenger (skipping vehicle).
    - Button: **"Finish"** -> Shows the summary screen.
6.  **[NEW] Attendance Summary**:
    - Displays all scanned users.
    - Annotates vehicle status (e.g., "Mary (Scanned In) - John's vehicle").
    - Button: **"Finalize & Save"** -> Pushes all logs to `access_logs` with server time.

## Proposed Changes

### Component: State Management

#### [MODIFY] [ScannerViewModel.kt](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/java/com/openscansa/app/viewmodel/ScannerViewModel.kt)
- Add `attendanceSession`: A data structure to hold the driver, a list of passengers, the vehicle data, and the session type (In/Out).

### Component: UI Layouts

#### [NEW] [fragment_attendance_vehicle_prompt.xml](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/res/layout/fragment_attendance_vehicle_prompt.xml)
- "Did you come with a vehicle?" question.
- Large "Yes" and "No" buttons.

#### [NEW] [fragment_attendance_summary.xml](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/res/layout/fragment_attendance_summary.xml)
- Scrollable list showing all participants.
- Distinct labels for the driver vs passengers.
- "Finalize" button.

### Component: Navigation

#### [MODIFY] [nav_graph.xml](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/res/navigation/nav_graph.xml)
- [NEW] `attendanceVehiclePromptFragment`
- [NEW] `attendanceSummaryFragment`
- Add actions to support the loop between verification and participant hub.

### Component: Logic Implementation

#### [MODIFY] [ActionHubFragment.kt](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/java/com/openscansa/app/ui/ActionHubFragment.kt)
- Update verification success to start the `attendanceSession` and navigate to the vehicle prompt.
- Handle state mismatch alerts ("User not scanned out/in") for every participant in the group.

#### [NEW] [AttendanceVehiclePromptFragment.kt](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/java/com/openscansa/app/ui/AttendanceVehiclePromptFragment.kt)
- Logic to branch between vehicle scanning and passenger addition.

#### [NEW] [AttendanceSummaryFragment.kt](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/java/com/openscansa/app/ui/AttendanceSummaryFragment.kt)
- Final step: Pushes all session records to `access_logs` in a single transaction-like batch.

## Verification Plan

### Manual Verification
1. **Full Group In**: Driver (QR+Pass) -> Vehicle Yes (Scan) -> Add Passenger -> Passenger (QR+Pass) -> Finish -> Verify Summary shows both.
2. **Vehicle Mismatch**: Scan Driver A -> Scan Driver B's vehicle -> Verify it displays "Came with Driver B's vehicle".
3. **Expired Vehicle**: Scan an expired disc during attendance -> Verify it blocks the process just like in profile setup.
4. **Mismatch Alerts**: Scan someone "In" who is already marked as "In" -> Verify the popup appears.
