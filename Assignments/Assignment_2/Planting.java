// Name: Felix Singerman
// Student id: 7970742
//
// The Planting Synchronization Problem
//
import java.util.concurrent.Semaphore;

public class Planting 
{
	public static void main(String args[]) 
	{ 
		// Create Student, TA, Professor threads
		TA ta = new TA();
		Professor prof = new Professor(ta);
		Student stdnt = new Student(ta);

		// Start the threads
		prof.start();
		ta.start();
		stdnt.start();

		// Wait for prof to call it quits
		try {prof.join();} catch(InterruptedException e) { }; 
		// Terminate the TA and Student Threads
		ta.interrupt();
		stdnt.interrupt();
	}   
}

class Student extends Thread
{
	TA ta;

	public Student(TA taThread)
	{
		ta = taThread;
	}

	public void run()
	{
		while(true)
		{

			System.out.println("Student: Must wait for TA "+ta.getMAX()+" holes ahead");

			try {
				ta.digStudent.acquire();
				try {
					ta.waitShovel.acquireUninterruptibly();
					// Can dig a hole - lets get the shovel
					System.out.println("Student: Got the shovel");

					try {sleep((int) (100*Math.random()));} catch (Exception e) { break;} // Time to fill hole
					ta.incrHoleDug();  // hole filled - increment the number	
					System.out.println("Student: Hole "+ta.getHoleDug()+" Dug");

					System.out.println("Student: Letting go of the shovel");
				}finally{
					ta.waitShovel.release();
				}

			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}finally{
				ta.waitProf.release();
			}

			if(isInterrupted()) break;

		}
		System.out.println("Student is done");
	}
}

class TA extends Thread
{
	// Some variables to count number of holes dug and filled - the TA keeps track of things
	private int holeFilledNum=0;  // number of the hole filled
	private int holePlantedNum=0;  // number of the hole planted
	private int holeDugNum=0;     // number of hole dug
	private final int MAX=5;   // can only get 5 holes ahead
	// add semaphores - the professor lets the TA manage things.
	final Semaphore digStudent;
	final Semaphore waitShovel;
	final Semaphore waitTa;
	final Semaphore waitProf;

	public int getMAX() { return(MAX); } 
	public void incrHoleDug() { holeDugNum++; }
	public int getHoleDug() { return(holeDugNum); }
	public void incrHolePlanted() { holePlantedNum++; }
	public int getHolePlanted() { return(holePlantedNum); }

	public TA()
	{
		// Initialize things here
		waitShovel = new Semaphore(1,true);
		digStudent = new Semaphore(MAX,true);
		waitTa = new Semaphore(0);
		waitProf = new Semaphore(0);
	}

	public void run()
	{
		while(true)
		{

			try {
				waitTa.acquire();
				try {
					waitShovel.acquireUninterruptibly();
					System.out.println("TA: Got the shovel");

					try {sleep((int) (100*Math.random()));} catch (Exception e) { break;} // Time to fill hole
					holeFilledNum++;  // hole filled - increment the number	
					System.out.println("TA: The hole "+holeFilledNum+" has been filled");
					System.out.println("TA: Letting go of the shovel"); //when we can give shovel
				}finally {
					waitShovel.release();
				}
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}finally {

				digStudent.release();

			}

			if(isInterrupted()) break;
		}
		System.out.println("TA is done");
	}
}

class Professor extends Thread
{
	TA ta;

	public Professor(TA taThread)
	{
		ta = taThread;
	}

	public void run()
	{
		while(ta.getHolePlanted() <= 20)
		{
			try {
				ta.waitProf.acquire();
				try {sleep((int) (50*Math.random()));} catch (Exception e) { break;} // Time to plant
				ta.incrHolePlanted();  // the seed is planted - increment the number	
				System.out.println("Professor: All be advised that I have completed planting hole "+
						ta.getHolePlanted());

			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}finally{
				ta.waitTa.release();
			}

		}

		System.out.println("Professeur: We have worked enough for today");

	}
}
