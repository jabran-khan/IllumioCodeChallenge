# Illumio Coding Assignment 2019-2020

## Author
Jabran Khan
Computer Science --- University of Waterloo
[Linkedin](#https://www.linkedin.com)

## Team Preference
Here is my ranking for team preference from most preferred to least preferred
1. Platform Team
2. Data Team
3. Policy Team

## How to Run
#### Requires g++ to be installed. Recommended with C++14 or higher.
1. Clone/download the repository
2. Compile using `make` in the terminal
3. Run using `./illumio`

## Coding Challenge Design

#### Design Structure
My focus was on making `acceptPacket` as efficient as possible. Thus, a lot of work is done during the `constructor` in order to correctly setup the data structures to quickly parse through and get the correct rule. The combination of classes make a  Trie-like data structure. Each part of the network packet, if matched correctly, leads to the next set of rules that need to be matched.

`Firewall` Class
- has a 4 lists of `PortIntervals`, one for each combination of protocol and direction.
	- combines `PortIntervals` that overlap with one another, keeps list as small as possible
	- each list is sorted from smallest to largest by the starting port of each interval
- parses through each rule and breaks them up into it's components to be placed into the data structure

I chose to make a 2 additional data structures to represent the intervals for the Ports called `PortInterval` and for the IP Addresses called `IPAddressInterval`. 

`PortInterval` Class
- two `int` to represent the min and max of each interval
- has an array data structure `listAddresses` which stores a sorted list of `IPAddressInterval` (sorted by starting address) which are valid addresses that under the current port
	-  Uses an algorithm to combine overlapping address intervals. 

`IPAddressInterval` Class
- Stores two arrays of length 4 (one for each octet) which represent the starting and ending address in the interval
	- Made this choice to make it easy to manage and perform operations on and across each octet such as `less than` and `greater than`. 

#### Flow for `Firewall` Constructor
- Read each line of `CSV` file
- Parse each rule, separate into `dir`, `protocol`, `ports` and `IP Addresses`
- Store `PortInterval` into correct list depending on `dir` and `protocol` 
- Store `IPAddressInterval` into correct `PortInterval`

#### Flow for `acceptPacket`
- Read `dir`, `protocol`, `port` and `IP Address`
- Check appropriate list depending on `dir` and `protocol`
- Check if port fits in any `PortInterval`.
- Check if IP Address fits in any `IPAddressInterval`

#### Testing
Unfortunately, due to time constrictions I was not able to test as thoroughly as I wanted since I also ran across a couple of bugs along the way.
I tested against the sample test suite that was given.
I also created additional tests to ensure that intervals were being correctly combined and remained in sorted order based on their starting port/address.

#### Improvements
There are many improvements that I'd like to make if more time was provided and allocated.
- When searching for a matching port or IP address in a list, I used a linear O(n) search using a `for-loop` to save time and code length. I would have liked to used `binary-search` to more efficiently find the correct intervals and rules.
- When combining two `PortIntervals`, I also needed to combine both interval's stored list of `IPAddressIntervals`. To save time, I simply concatenated both `IPAddressIntervals`. I would have liked to properly go through the lists and combine any intervals that overlapped. This would make searching faster since there would be less elements in the lists.
- Take advantage of C++ operator overloading instead of creating separate comparison functions.
- Use appropriate error handling methodologies and catch the proper exceptions.
- Create command-line functionalities such as reading the `CSV` instead of hard-coding it in the program `main`