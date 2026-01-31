import java.util.Random;
import java.time.Clock;
import java.time.Instant;
import java.time.Duration;

public class Coordinator {

    public static void main(String[] args) {

        // I expect EXACLTY 2 command line arguments:
        // args[0] = Seed       (for buffer size, #items, #threads)
        // args[1] = alphabetSeed (for the random letters A–Z)
        if (args.length != 2) {
            System.err.println("Usage: java Coordinator <Seed> <alphabetSeed>");
            return;
        }

        // First argument: used to decide sizes and counts
        long seed = Long.parseLong(args[0]);
        // Second argument: used by producers to generate letters
        long alphabetSeed = Long.parseLong(args[1]);

        //random object using the Seed for buffer size, # of items, # of producers, and consumers
        Random rng = new Random(seed);


        int bufferSize   = 10 + rng.nextInt(6);  // 10–15
        int totalItems   = 20 + rng.nextInt(21); // 20–40
        int numProducers = 3  + rng.nextInt(5);  // 3–7
        int numConsumers = 3  + rng.nextInt(5);  // 3–7

        //print out the setup so it looks like the example output
        System.out.printf("[Coordinator] Buffer Size: %d%n", bufferSize);
        System.out.printf("[Coordinator] Total Items: %d%n", totalItems);
        System.out.printf("[Coordinator] No. of Producer: %d%n", numProducers);
        System.out.printf("[Coordinator] No. of Consumers: %d%n", numConsumers);

        // shared circular buffer that everybody uses :)
        Buffer buffer = new Buffer(bufferSize);

        // holds Producer and Consumer objects
        Producer[] producers = new Producer[numProducers];
        Consumer[] consumers = new Consumer[numConsumers];

        // holds actual Thread objects that run them
        Thread[] producerThreads = new Thread[numProducers];
        Thread[] consumerThreads = new Thread[numConsumers];

        //divide work between producer
        //base number of items each producer will handle
        int baseProd = totalItems / numProducers;

        //remainder goes to last producer
        int remProd  = totalItems % numProducers;

        for (int i = 0; i < numProducers; i++) {
            int itemsForThisProducer = baseProd;
            if (i == numProducers - 1) {
                //Last producer takes any leftover items
                itemsForThisProducer += remProd;
            }

            // create the Producer (implements Runnable)
            producers[i] = new Producer(buffer, itemsForThisProducer, i + 1, alphabetSeed);

            // wrap it in a Thread so I can call start() and join() on it
            producerThreads[i] = new Thread(producers[i], "Producer-" + (i + 1));
        }

        //divide work between consumers
        //split items evenly, last one grabs leftovers
        int baseCons = totalItems / numConsumers;
        int remCons  = totalItems % numConsumers;

        for (int i = 0; i < numConsumers; i++) {
            int itemsForThisConsumer = baseCons;
            if (i == numConsumers - 1) {
                itemsForThisConsumer += remCons;
            }

            //create the Consumer
            consumers[i] = new Consumer(buffer, itemsForThisConsumer, i + 1);

            //wrap it in a Thread
            consumerThreads[i] = new Thread(consumers[i], "Consumer-" + (i + 1));
        }

        //start all Producer threads
        for (Thread t : producerThreads) {
            t.start();
        }

        //start all Consumer threads
        for (Thread t : consumerThreads) {
            t.start();
        }

        // wait (join) for everyone to finish
        // main waits until producers and consumers done
        try {
            for (Thread t : producerThreads) {
                t.join();
            }
            for (Thread t : consumerThreads) {
                t.join();
            }
        } catch (InterruptedException e) {
            // exit if flagged
            Thread.currentThread().interrupt();
        }

        //sum up consonants from all producers and consumers
        int totalGeneratedConsonants = 0;
        for (Producer p : producers) {
            totalGeneratedConsonants += p.getNumOfGeneratedConsonants();
        }

        int totalConsumedConsonants = 0;
        for (Consumer c : consumers) {
            totalConsumedConsonants += c.getNumOfConsumedConsonants();
        }

    
        // if the totals match, everything worked!
        // every consonant produced got consumed exactly once :))
        if (totalGeneratedConsonants == totalConsumedConsonants) {
            System.out.printf(
                "The generated & consumed numbers of consonants are the same as shown: %d%n",
                totalGeneratedConsonants
            );
        } else {
            // If this ever prints, something is off in the synchronization / logic
            System.out.printf(
                "Mismatch! Generated consonants: %d, Consumed consonants: %d%n",
                totalGeneratedConsonants,
                totalConsumedConsonants
            );
        }
    }

    //Call this function from your generator or your consumer to get the time stamp to be displayed
    public static String getTime() {
        Clock offsetClock = Clock.offset(Clock.systemUTC(), Duration.ofHours(-9));
        Instant time = Instant.now(offsetClock);
        String timeString = time.toString();
        timeString = timeString.replace('T', ' ');
        timeString = timeString.replace('Z', ' ');
        return timeString;
    }
}
