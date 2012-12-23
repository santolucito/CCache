public class Trace{

  char tagCF;
  String pageIn;
  int cSize;
  int cTime;
  char tagWN;
  String pageOut;
  int dSize;
  int dTime;

  public Trace (char tagCF, String pageIn, int cSize, int cTime, char tagWN, String pageOut, int dSize, int dTime){
    this.tagCF=tagCF;
    this.pageIn=pageIn;
    this.cSize=cSize;
    if(cTime<0){
	System.out.println(pageIn);    
	this.cTime=10000;//just a bad fix for now, something is wrong with the times, getting something like -9999...
	//System.exit(0);
    }
    else{
	this.cTime=cTime;
    }
    this.tagWN=tagWN;
    this.pageOut=pageOut;
    this.dSize=dSize;
    this.dTime=dTime;

  }

  public String toString(){
    return "( "+tagCF+" "+pageIn+" "+cSize+" "+cTime+" "+tagWN+" "+pageOut+" "+dSize+" "+dTime+" )";
  }

}
