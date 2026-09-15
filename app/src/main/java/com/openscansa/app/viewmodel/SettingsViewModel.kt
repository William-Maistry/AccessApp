package com.openscansa.app.viewmodel

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import com.openscansa.app.repository.SettingsRepository

class SettingsViewModel(application: Application) : AndroidViewModel(application) {
    private val repository = SettingsRepository(application)
    private val _torchEnabled = MutableLiveData(repository.torchEnabled)
    val torchEnabled: LiveData<Boolean> = _torchEnabled

    fun setTorchEnabled(enabled: Boolean) {
        repository.torchEnabled = enabled
        _torchEnabled.value = enabled
    }
}
