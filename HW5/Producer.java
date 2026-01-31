import java.util.Random;

public class Producer extends Thread {

    //Necessary variables and object declaration
    private final Buffer buff; //shared buffer
    private final int count;
    private final int id;
    private int numOfConsonants;

    private final Random randomWithSeed;

    //assignment recommended it...
    private static final String UNDERLINE = "\033[0;4m";
    private static final String RESET     = "\033[0;0m";

    public Producer(Buffer buff, int count, int id, long alphabetSeed) {
       	//Assign values to the variables
        this.buff = buff;
        this.count = count;
        this.id = id;
        this.numOfConsonants = 0;

        this.randomWithSeed = new Random(alphabetSeed);
    }

    //helper -> char consonant??
    private static boolean isConsonant(char c) {
        return "AEIOU".indexOf(c) == -1;
    }

    // called by coordinator
    public int getNumOfGeneratedConsonants() {
        return numOfConsonants;
    }

    @Override
    public void run() {
      
        for (int i = 0; i < count; i++) {

            //gen a random CAPITAL letter A–Z
            char alphabet = (char) ('A' + randomWithSeed.nextInt(26));

            // keep track of consonants
            if (isConsonant(alphabet)) {
                numOfConsonants++;
            }

            try {
                // try inserting into shared buffer
                // will wait if full
                int index = buff.insert(alphabet, id);

                //again its about the format baby!
                String time = Coordinator.getTime();

                System.out.printf(
                    UNDERLINE + "[Producer %d]: inserted %2c at index %2d at time %s" 
                    + RESET + "%n",
                    id, alphabet, index, time
                );

                System.out.flush(); 

            } catch (InterruptedException e) {
                // if thread is interrupted, ABORT
                Thread.currentThread().interrupt();
                break;
            }
        }
    }
}
