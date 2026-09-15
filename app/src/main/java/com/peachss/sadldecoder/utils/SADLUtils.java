package com.peachss.sadldecoder.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;

import java.io.ByteArrayOutputStream;

public class SADLUtils {

    public static Bitmap getBitmapFromImageBytes(byte[] imageData) {
        Bitmap b = Bitmap.createBitmap(200, 250, Bitmap.Config.ARGB_8888);
        int h = 0;
        int w = 0;
        int p = 50000;
        while (p > 0) {
            p--;
            int color = (255 & 0xff) << 24 | (imageData[p] & 0xff) << 16 | (imageData[p] & 0xff) << 8 | (imageData[p] & 0xff);
            b.setPixel(199 - w, h, color);
            w++;
            if (w >= 200) {
                h++;
                w = 0;
            }
        }
        return b;
    }


    public static Bitmap convertFromBase64(String base64Str) throws IllegalArgumentException
    {
        byte[] decodedBytes = Base64.decode(
                base64Str.substring(base64Str.indexOf(",")  + 1),
                Base64.DEFAULT
        );

        return BitmapFactory.decodeByteArray(decodedBytes, 0, decodedBytes.length);
    }

    public static String convertToBase64(Bitmap bitmap)
    {
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.PNG, 100, outputStream);

        return Base64.encodeToString(outputStream.toByteArray(), Base64.NO_WRAP);
    }

}
