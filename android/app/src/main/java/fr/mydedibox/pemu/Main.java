package fr.mydedibox.pemu;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.libsdl.app.SDLActivity;

public class Main extends SDLActivity {
    String dataPath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // set data path
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            dataPath = getExternalFilesDir(null).getAbsolutePath();
        } else {
            dataPath = getFilesDir().getAbsolutePath();
        }

        // extract libcross2d "romfs"
        if (!isDataUpToDate("romfs")) {
            copyAssets("romfs");
        }

        // extract libcross2d "data"
        if (!isDataUpToDate("data")) {
            copyAssets("data");
        }

        super.onCreate(savedInstanceState);
    }

    @Override
    protected String[] getLibraries() {
        return new String[]{
                "psnes"
        };
    }

    public boolean isDataUpToDate(String path) {
        String storageVersion, assetVersion;
        try {
            byte[] bytes = Files.readAllBytes(Paths.get(dataPath + "/" + path + "/version"));
            storageVersion = new String(bytes).trim();
        } catch (IOException e) {
            return false;
        }

        try {
            InputStream is = getAssets().open(path + "/version");
            BufferedReader br = new BufferedReader(new InputStreamReader(is, StandardCharsets.UTF_8));
            assetVersion = br.readLine().trim();
            br.close();
        } catch (IOException e) {
            return false;
        }

        Log.e("C2D", path + ": asset version: "
                + assetVersion + ", storage version: " + storageVersion);

        return assetVersion.equals(storageVersion);
    }

    public boolean copyAssets(String path) {
        //Log.e("C2D", "getRomFs: " + path);
        try {
            String[] list = getAssets().list(path);
            if (list.length > 0) {
                create(dataPath + "/" + path);
                for (String file : list) {
                    if (!copyAssets(path + "/" + file)) {
                        return false;
                    } else {
                        String src = path + "/" + file;
                        String dst = dataPath + "/" + src;
                        if (isFile(src)) {
                            copy(src, dst);
                        }
                    }
                }
            }
        } catch (IOException e) {
            return false;
        }

        return true;
    }

    private boolean isFile(String asset) {
        try {
            InputStream in = getAssets().open(asset);
            in.close();
        } catch (IOException e) {
            return false;
        }

        return true;
    }

    private void create(String path) {
        File dir = new File(path);
        if (!dir.exists()) {
            Log.e("C2D", "create: " + dir.getAbsolutePath());
            dir.mkdirs();
        }
    }

    private void copy(String asset, String dst) throws IOException {
        Log.e("C2D", "copy: " + asset + " => " + dst);
        InputStream in = null;
        OutputStream out = null;

        try {
            byte[] buffer = new byte[1024 * 4];
            int read;
            in = getAssets().open(asset);
            out = new FileOutputStream(dst);
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
        } catch (IOException e) {
            Log.e("C2D", "copy failed: " + dst);
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    // NOOP
                }
            }
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    // NOOP
                }
            }
        }
    }
}
