package com.peachss.sadldecoder;


import java.io.UnsupportedEncodingException;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;

import android.graphics.Bitmap;
import android.util.Base64;

import com.peachss.sadldecoder.utils.LicenseInfo;
import com.peachss.sadldecoder.utils.SADLUtils;


public class Decoder {

    public static int[] hexToIntArray(String s) {
        int len = s.length();
        int[] data = new int[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16)) & 0xFF;
        }
        return data;
    }

    public static byte[] hexToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    /**
     * Decodes the Byte array of a South African Drivers License into the different fields as stiputlated in
     * LicenseInfo
     *
     * @param data byte[]
     * @return LicenseInfo
     * @throws UnsupportedEncodingException
     */
    public static LicenseInfo decodeLicenseBarcode(byte[] data) throws UnsupportedEncodingException {
        return new Decoder().decryptLicense(data);
    }

    private static boolean initialized=false;
    public  static void  initialize()
    {
        if (initialized)
            return;
        try {
            System.loadLibrary("SWIDecoder");
            initialized=true;
        } catch (UnsatisfiedLinkError e) {
            android.util.Log.e("Decoder", "Failed to load SWIDecoder library", e);
        }
    }

    public static byte[] getDecodedPhoto(byte[] photoData){
        return  Decoder.getDecodedPhoto(photoData,photoData.length);
    }

    public static byte[] toPrimitives(Byte[] oBytes) {

        byte[] bytes = new byte[oBytes.length];
        for (int i = 0; i < oBytes.length; i++) {
            bytes[i] = oBytes[i];
        }
        return bytes;

    }

    public static Byte[] toObjects(byte[] bytesPrim) {

        Byte[] bytes = new Byte[bytesPrim.length];
        int i = 0;
        for (byte b : bytesPrim) bytes[i++] = b; //Autoboxing
        return bytes;

    }

    private LicenseInfo decryptLicense(byte[] data) throws UnsupportedEncodingException {
        int size = 720;
        int version = readVersion(data);
        size -= 6;
        data = Arrays.copyOfRange(data, 6, 720);
        int pos = 0;
        List<Byte> buffers = new ArrayList<Byte>();

        while (size > 0) {
            int readsize = 74;
            if (size > 74) {
                readsize = 128;
            }
            byte[] result = doDecrypt(Arrays.copyOfRange(data, pos, pos + readsize), version, readsize);
            for (byte b : result) {
                buffers.add(b);
            }
            ;
            size -= readsize;
            pos += readsize;
        }

        byte[] result = toPrimitives(buffers.toArray(new Byte[buffers.size()]));
        return processLicenseInfo(result);
    }


    private LicenseInfo processLicenseInfo(byte[] data) throws UnsupportedEncodingException {
        byte[] actualData = new byte[684];
        int pos = 0;
        //Skip first 5 bytes of each block
        while (data.length > 0 && pos < 684) {
            if (data.length > 128) {
                System.arraycopy(data, 5, actualData, pos, 128 - 5);
                data = Arrays.copyOfRange(data, 128, data.length);

                pos += 123;
            } else {
                System.arraycopy(data, 5, actualData, pos, 74 - 5);
                data = Arrays.copyOfRange(data, 74, data.length);
                pos += 69;
            }
        }
        //data = Arrays.copyOfRange(actualData, 0, actualData.length);
        Byte s1Len = actualData[5];
        Byte s2Len = actualData[7];
        int s3Len = actualData.length - s1Len - s2Len;
        LicenseInfo li = new LicenseInfo();

        byte[] s1Str = Arrays.copyOfRange(actualData, 10, s1Len + 10);
        byte[] s2Str = Arrays.copyOfRange(actualData, 10 + s1Len, 10 + s1Len + s2Len);
        byte[] s3Str = Arrays.copyOfRange(actualData, 10 + s1Len + s2Len, s1Len + s2Len + s3Len);

        String SS3Str = Decoder.bytesToHex(s3Str);
        int fieldNo = 1;
        int charPos = 0;
        List<Byte> charValue = new ArrayList<Byte>();

        for (byte i : s1Str) {
            if (i == -32 || i == -31) {
                if (charPos > 0) {
                    assignValue(li, fieldNo, new String(toPrimitives(charValue.toArray(new Byte[charValue.size()])), "ASCII"));
                    if (i == -31)
                        fieldNo++;
                }
                charValue.clear();
                charPos = 0;
                fieldNo++;
            } else {
                charValue.add(i);
                charPos++;
            }
        }

        assignValue(li, fieldNo, new String(toPrimitives(charValue.toArray(new Byte[charValue.size()])), "ASCII"));
        fieldNo++;
        String s2Hex = bytesToHex(s2Str);
        int nibbleOffSet = 0;

        for (fieldNo = 16; fieldNo < 16 + 12; fieldNo++) {
            int nibbleSize = 8;
            if (fieldNo == 16 || fieldNo == 21 || fieldNo == 23 || fieldNo == 27)
                nibbleSize = 2;
            String value = getNibbles(s2Hex, nibbleOffSet, nibbleSize);
            if (value == null || value.isEmpty())
                nibbleOffSet++;
            else {
                assignValue(li, fieldNo, value);
                nibbleOffSet += nibbleSize;
            }
        }
        assignValue(li, 28, s3Str);

        return li;
    }

    private Date getDateFromValue(String value) {
        DateFormat format = new SimpleDateFormat("yyyyMMdd");
        Date result;
        try {
            result = format.parse(value);
            return result;
        } catch (ParseException e) {
            //return Date.from(null);
            throw new NullPointerException();
        }
    }

    private void assignValue(LicenseInfo info, int fieldCnt, Object value) {
        switch (fieldCnt) {
            case 1:
                info.setLicenseCode1(value.toString());
                break;
            case 2:
                info.setLicenseCode2(value.toString());
                break;
            case 3:
                info.setLicenseCode3(value.toString());
                break;
            case 4:
                info.setLicenseCode4(value.toString());
                break;
            case 5:
                info.setSurname(value.toString());
                break;
            case 6:
                info.setInitials(value.toString());
                break;
            case 7:
                info.setPdpCode(value.toString());
                break;
            case 8:
                info.setIdCountryOfIssue(value.toString());
                break;
            case 9:
                info.setLicenseCountryOfIssue(value.toString());
                break;
            case 10:
                info.setVehicleRestriction1(value.toString());
                break;
            case 11:
                info.setVehicleRestriction2(value.toString());
                break;
            case 12:
                info.setVehicleRestriction3(value.toString());
                break;
            case 13:
                info.setVehicleRestriction4(value.toString());
                break;
            case 14:
                info.setLicenseNumber(value.toString());
                break;
            case 15:
                info.setIdNumber(value.toString());
                break;
            case 16:
                info.setIdType(value.toString());
                break;
            case 17:
                info.setLicenseIssueDate1(getDateFromValue(value.toString()));
            case 18:
                info.setLicenseIssueDate2(getDateFromValue(value.toString()));
            case 19:
                info.setLicenseIssueDate3(getDateFromValue(value.toString()));
            case 20:
                info.setLicenseIssueDate4(getDateFromValue(value.toString()));
            case 21:
                info.setDriverRestriction(value.toString());
                break;
            case 22:
                info.setPdpExpiry(getDateFromValue(value.toString()));
                break;
            case 23:
                info.setLicenseIssueNo(value.toString());
                break;
            case 24:
                info.setBirthDate(getDateFromValue(value.toString()));
                break;
            case 25:
                info.setLicenseValidFrom(getDateFromValue(value.toString()));
                break;
            case 26:
                info.setLicenseValidTo(getDateFromValue(value.toString()));
                break;
            case 27:
                if (value.toString().compareTo("01") == 0)
                    info.setGender("M");
                else
                    info.setGender("F");
                break;
            case 28:
                info.setRawImage(toObjects((byte[]) value));
                break;
            case 29:
                info.setDecompressedImage(toObjects((byte[]) value));
                break;
            case 30:
                //info.setPhoto((Bitmap) value);
                break;
        }
    }


    private String getNibbles(String buffer, int nibbleOffSet, int nibbleCnt) {
        int bufPos = 0;

        String twoNibbles = buffer.substring(nibbleOffSet + bufPos, nibbleOffSet + bufPos + 2);

        int intNibbleCnt = 0;

        if (twoNibbles.getBytes()[0] == 'A') {
            return null;
        } else {
            List<Byte> retVal = new ArrayList<Byte>();
            while (intNibbleCnt < nibbleCnt) {
                twoNibbles = buffer.substring(nibbleOffSet + bufPos, nibbleOffSet + bufPos + 2);
                bufPos += 2;
                retVal.add(twoNibbles.getBytes()[0]);
                intNibbleCnt++;
                if (intNibbleCnt <= nibbleCnt)
                    retVal.add(twoNibbles.getBytes()[1]);
                intNibbleCnt++;
            }
            try {
                return new String(toPrimitives(retVal.toArray(new Byte[retVal.size()])), "ASCII");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            } // retVal.ToArray().ByteToString();
        }
        return "";
    }

    private int readVersion(byte[] br) {
        ByteBuffer wrapped = ByteBuffer.wrap(Arrays.copyOfRange(br, 0, 4)).order(ByteOrder.LITTLE_ENDIAN);
        int val = wrapped.getInt();

        if (val == 1157816577) {
            return 1;
        }
        if (val == 1158257409 || val == 1158208257) {
            return 2;
        }
        
        // Fallback: search for the 0x45 version marker in the first few bytes
        for (int i = 0; i < 6; i++) {
            if ((br[i] & 0xFF) == 0x45) {
                return 2;
            }
        }

        return 2;
    }

    private byte[] doDecrypt(byte[] buffer, int version, int size) {
        //RSAKeyParameters o = null;
        BigInteger e = null;
        BigInteger m = null;
        if (version == 2) {
            if (size == 128) {
                m = new BigInteger(Base64.decode("AMqfGO9sPz+kxaRh/qVKsZQGul7NdG1gonSS3KPXTjtcHTFfexA4MkGAmwKeu9XeTRFgMMxX99WmyaFvNzuxSlCFI/foCkx0TZCFZjpKFHLXryxWrkG1Bl9++gKTvTJ4rWk1RvnxYhm3n/Rxo2NoJM/822Oo7YBZ5rmk8NuJU4HL", Base64.DEFAULT));
                e = new BigInteger(Base64.decode("GHCS2mRUzrGFPmkV+EZqBQ==", Base64.DEFAULT));
            } else if (size == 74) {
                m = new BigInteger(Base64.decode("ALQEoN8R0crP8aGgSNTVc/lTpixYPXSSWSdWGm16HisUBCUmr3C1UFRzkOpux0jTD9uBrbSQ4MNqGYa0BLL19p712htmPllQkTDn", Base64.DEFAULT));
                e = new BigInteger(Base64.decode("MJz+2XGf4qXiDJu0R2U4Kw==", Base64.DEFAULT));
            }
        } else {
            if (size == 128) {
                m = new BigInteger(Base64.decode("AP7S4cJ+M2MxbncxenpSxUmBOVGGvkl0dgxyUY1j4FRKSNCIszLFsMNwx2XWXZg8H53gpCsxDMwHrncL0rYdak3M6sdXaJvcv2CEePrzEvYIfMSWw3Ys9cRlHK7No0mfrn7bfrQOPhjrMEFw6R7VsVaqzm9DLW7KbMNYUd6MZ49n", Base64.DEFAULT));
                e = new BigInteger(Base64.decode("ALt5f/3sf55Cydb3mxNwWds=", Base64.DEFAULT));
            } else if (size == 74) {
                m = new BigInteger(Base64.decode("AP887GtfQOPDZhRRufz67zrrBtwjKcDm9NzMknlyZxbOFbvgXu0sVxG8+PW2yPcnbbXEO/qjBA3AGrFLnE0W9xwM5eqVPwx1TGsX", Base64.DEFAULT));
                e = new BigInteger(Base64.decode("ANsFuoItmswz+rfY9Cf5zmU=", Base64.DEFAULT));
            }

        }
        byte[] decrypted = encryptValue(buffer, e, m, size);
        return decrypted;
    }

    private byte[] getPaddedValue(BigInteger value, int length) {
        byte[] result = value.toByteArray();
        return result;
    }

    private byte[] encryptValue(byte[] rgb, BigInteger e, BigInteger n, int size) {
        BigInteger input = new BigInteger(1, rgb);
        BigInteger output = input.modPow(e, n);

        byte[] result = getPaddedValue(output, size);
        return result;
    }


    /**
     * A native method that is implemented by the 'SWIDecoder' native library,
     * which is packaged with this application.
     */
    public static native byte[] getDecodedPhoto(byte[] photoData, int size);
}
