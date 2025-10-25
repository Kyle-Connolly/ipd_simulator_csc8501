================================================
CSC8501 Assignment - Iterated Prisoner’s Dilemma
================================================
Assessment Brief & Tasks:
Implement a fully functional Iterated Prisoner’s Dilemma (IPD) simulator in C++. This simulator will support the execution of tournaments between different strategies, produce results in multiple formats, and allow experimentation with noise, payoffs, and evolutionary dynamics. The assessment consists of building the simulation engine, implementing the required set of strategies plus two original ones, and completing a set of experiments to demonstrate and analyse the behaviour of your simulator and strategy.
•	Implement a match engine that runs repeated rounds of IPD between two strategies.
•	Support tournament play: double round-robin, multiple repeats, fresh state each match.
•	Provide a unified CLI with options to control rounds, repeats, noise, payoffs, strategies, output format, saving/loading configs, and running evolutionary experiments.
•	Implement required strategies: ALLC, ALLD, TFT, GRIM, PAVLOV, RND(p), CONTRITE TFT, PROBER, plus two original strategies of your own design.

Instructions:
•	Use the developer command prompt within Visual Studio to enter commands and run the program.
•	You must navigate to the x64 > Debug folder, for example: cd C:\...\...\x64\Debug.
•	The program can be run by typing "csc8501-ipd-200982173.exe" followed by the desired commands. Note that some commands must be supplied together.
•	The --payoff field can be changed if desired, though certain rules must be followed as you will see with the error messages. The default payoff values are: 5,3,1,0.
•	Note to print relevant info to the console instead of a csv, enter "--format text" instead of "--format csv".
•	An example command: csc8501-ipd-200982173.exe --rounds 10 --repeats 1 --strategies ALLD,TFT,GRIM,PROBER --epsilon 0.2 --seed 5 --evolve 1 --population 100 --generations 50 --format csv --scb 1.
•	Please find the assignment documentation file within this repository for more details on the design and development of this project.
