# CSCI-567: Artificial Intelligence Assignment 2: Checkers AI

This program is an artificial intelligence agent with the goal to play a game of checkers.
The agent uses mini-max along with alpha-beta pruning to explore the search space. It evaluates a given position using a combination of weights for various features such as the number of pieces remaining, how many pieces are vulnerable, what type of pieces are remaining, etc.
The evaluation scores were initially created by hand, while referencing wikipedia and scholarly articles on checkers evaluation. Grid-search was then performed alongside testing to determine the best weights.

This agent was pit against 330 other agents created by fellow classmates during the course of the semester. It achieved an overall 5th place in a weighted win-draw-loss format.