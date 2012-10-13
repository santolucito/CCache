import java.util.*;

public class Analyze{

public static void main(String args[]){
  
  ArrayList<Trace> LZOdata = Reader.read("../LZO.raw");
  ArrayList<Trace> LZRW1data = Reader.read("../LZRW1.raw");
  ArrayList<Trace> WK2data = Reader.read("../WK2.raw");
  ArrayList<Trace> WKdmdata = Reader.read("../WKdm.raw");
 
  System.out.println("LZO average compression time " + average(LZOdata));
  System.out.println("LZRW1 average compression time " + average(LZRW1data));
  System.out.println("WK2 average compression time " + average(WK2data));
  System.out.println("WKdm average compression time " + average(WKdmdata));
}//main

public static double average(ArrayList<Trace> traces){
  
  int sumCTimes = 0;
  int dataLength =0;
  for(Trace t:traces){
    sumCTimes+=t.cTime;
    dataLength++;
  }

  return (double)sumCTimes/dataLength;
}//average
 
}


