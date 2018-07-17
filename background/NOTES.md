# Notes of observations from annotated_trace

1 Colour pin(9) can and is left high or low for long periods

2 The Enable Pins all change with each commit

3 most sequences pulse row 1 or 2 times, and column rarely
 this implies that neither counter is reset by commit

4 if row is pulsed once per cycle, column is pulse every 16th

5 commit with column high and all 4 enables high is a column reset ?
  immediately followed by a standard commit
  examples: 959500

6 curious event at 1515500  really long pulse on P0+P1 whilst pulsing
  column and commit

