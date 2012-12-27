import java.io.*;
import java.util.*;

public class Reader{

public static void main(String[] args){
  
 if(args.length!=1){
	System.out.println("Usage: java Reader <uncompressed processed trace>");
	System.exit(0);
  } 

  ArrayList<Trace> traces = read(args[0]);
  
  long sumCTimes = 0;
  int dataLength =0;
  for(Trace t:traces){
    sumCTimes+=t.cTime;
    dataLength++;
    //if(dataLength%100==0)System.out.println(sumCTimes);
  }

  System.out.println(args[0]+"\naverage cTime (nsec): "+((double)sumCTimes/dataLength)+"\ntotal Time (microsec): "+sumCTimes/1000000.0+"\npage swaps: "+dataLength);
}//main()




  public static ArrayList<Trace> read(String filePath){
  
  ArrayList<Trace> allTraces = new ArrayList<Trace>();

  try{
  // Open the file that is the first 
  // command line parameter
  FileInputStream fstream = new FileInputStream(filePath);
  // Get the object of DataInputStream
  DataInputStream in = new DataInputStream(fstream);
  BufferedReader br = new BufferedReader(new InputStreamReader(in));
  String strLine;
//Read File Line By Line
  int numberOfLines =0;
  String[] data = new String[15];
  while ((strLine = br.readLine()) != null)   {
    //create objects
    if(numberOfLines==0) numberOfLines++; //skip first line
    else if(strLine.equals(")")){
      //System.out.println("read "+numberOfLines+" lines.");
      break; //reached the end
    }
    else{
      strLine.trim();
      data = strLine.split(" ");
      numberOfLines++;
      Trace t = new Trace(data[3].charAt(0),data[4],Integer.parseInt(data[5]),
    		          Integer.parseInt(data[6]),data[7].charAt(0),data[8],
   			  Integer.parseInt(data[9]),Integer.parseInt(data[10]));
      allTraces.add(t);
    }
  }
  //Close the input stream
  in.close();
    }catch (Exception e){//Catch exception if any
  System.err.println("Error: " + e.getMessage());
  }
  return allTraces;
}//read()


}
