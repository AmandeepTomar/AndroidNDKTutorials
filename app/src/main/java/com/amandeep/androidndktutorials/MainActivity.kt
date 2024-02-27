package com.amandeep.androidndktutorials

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.amandeep.androidndktutorials.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private var binding: ActivityMainBinding? = null

    companion object {

        init {
            System.loadLibrary("androidndktutorials")
        }
    }

    external fun getValues(): String

    external fun processString():String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding?.root)

        binding?.let { bindingValues ->
            bindingValues.btnValues.setOnClickListener {
                val valuesFromC = getValues()
                val fromProcessMethod = processString()
                Log.e("TAG", "onCreate: $fromProcessMethod", )
                bindingValues.tvValues.text = "$valuesFromC and $fromProcessMethod"

            }
        }
    }

    fun processInJava(): String {
        return "Process In Java"
    }
}