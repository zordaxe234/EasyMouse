import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.awt.Robot;
import java.util.Random;

import java.awt.AWTException;
import java.awt.MouseInfo;
import java.awt.Point;

public class EasyMouseServer {

    public static final int FIVE_SECONDS = 1000;
    public static final int MAX_Y = 400;
    public static final int MAX_X = 400;

    private Robot robot;
    // private Random random = new Random();

    public float mouseX = 0;
    public float mouseY = 0;
    public static final int MOUSE_SENSITIVITY = 5;
    public boolean clickReset = false;

    public EasyMouseServer() {
        try {
            robot = new Robot();
        } catch (AWTException e) {
            e.printStackTrace();
        }

        System.out.println("Waiting for clients to connect.");
        try (ServerSocket serverSocket = new ServerSocket(7500)) {
            while (true) {
                Socket socket = serverSocket.accept();
                System.out
                        .println("> Client connected: " + socket.getRemoteSocketAddress().toString().replace("/", ""));

                while (true) {
                    try {
                        byte[] cache = new byte[100];
                        socket.getInputStream().read(cache);
                        // The buffer is a fixed frame at 100 bytes. A message that is less than 100
                        // bytes will append
                        // the null terminator. We remove this character and replace it with the empty
                        // character.
                        String messageReceived = new String(cache, StandardCharsets.UTF_8).replace("\0", "");

                        String[] split = messageReceived.trim().split(" ");
                        // split[0] x
                        // split[1] y
                        // split[2] z
                        if (split.length == 3) {
                            float x = Float.parseFloat(split[0]);
                            float y = Float.parseFloat(split[1]);
                            float z = Float.parseFloat(split[2]);

                            final int IGNORE_VALUE = 5;
                            // 0ing out small values
                            if (Math.abs(x) < IGNORE_VALUE) {
                                x = 0;
                            } else {
                                if (x < 0)
                                    x += IGNORE_VALUE;
                                else
                                    x -= IGNORE_VALUE;
                            }
                            if (Math.abs(y) < IGNORE_VALUE) {
                                y = 0;
                            } else {
                                if (y < 0)
                                    y += IGNORE_VALUE;
                                else
                                    y -= IGNORE_VALUE;
                            }

                            if (z < 0)
                                z += 9.7;
                            else
                                z -= 9.7;
                            if (Math.abs(z) < IGNORE_VALUE) {
                                z = 0;
                            }
                            // adding the movement of the mouse
                            if (x == 0 && y == 0 && z == 0) { //no movement
                                continue;
                            } else if (clickReset == true && z!=0) { // we're clicking, not moving.
                                clickReset = false; //prevent double clicking
                                System.out.println("Received: " + x + " " + y + " " + z);
                                if (z > 0)
                                    System.out.println("Right Click");
                                else
                                    System.out.println("Left Click");
                            } else { // if z == 0, we are not clicking, but moving
                                clickReset = true;
                                Point mousePoint = MouseInfo.getPointerInfo().getLocation();
                                int currMouseX = mousePoint.x;
                                int CurrMouseY = mousePoint.y;
                                mouseX = currMouseX + (y * MOUSE_SENSITIVITY);
                                mouseY = CurrMouseY + (x * MOUSE_SENSITIVITY);
                                robot.mouseMove((int) mouseX, (int) mouseY);
                                System.out.println("Received: " + x + " " + y + " " + z);
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws IOException {
        new EasyMouseServer();
    }

}