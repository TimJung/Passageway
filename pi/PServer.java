import java.net.*;
import java.io.*;

public class PServer {
  public static void main (String[] a) {
    try {
      DatagramSocket sock = new DatagramSocket(5000);
      byte[] buff = new byte[1024];
      DatagramPacket rcv_packet = new DatagramPacket (buff, buff.length);
      String resp = "OK";
      DatagramPacket snd_packet;
      while (true) {
        sock.receive (rcv_packet);
        InetAddress addr = rcv_packet.getAddress();
        System.out.printf ("Received from %s:%d %s\n", addr.getHostAddress(),
          rcv_packet.getPort(), new String(rcv_packet.getData()));
        byte[] tmpBuff = resp.getBytes();
        snd_packet = new DatagramPacket (tmpBuff, tmpBuff.length, 
          addr, rcv_packet.getPort());
        sock.send (snd_packet);
      }
    }
    catch (IOException e) {
    }
  }
}
