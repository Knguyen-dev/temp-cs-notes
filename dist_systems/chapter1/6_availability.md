# Availability 
The time a system remains operational during a specific period of time.

### The Nine's of availability
You can calculate availability with $Availability = \frac{Uptime}{(Uptime+Downtime)}$. You generally measure availability in the number of 9s. For example, 99 percent availabile is said to have "2 nines" of availability. If it's 99.9% then that's "3 nines", and so on. I mean this stuff can add up:

| **Availability (%)**     | **Downtime/Year**  | **Downtime/Month** | **Downtime/Week**  |
| ------------------------ | ------------------ | ------------------ | ------------------ |
| 90% (one nine)           | 36.53 days         | 72 hours           | 16.8 hours         |
| 99% (two nines)          | 3.65 days          | 7.20 hours         | 1.68 hours         |
| 99.9% (three nines)      | 8.77 hours         | 43.8 minutes       | 10.1 minutes       |
| 99.99% (four nines)      | 52.6 minutes       | 4.32 minutes       | 1.01 minutes       |
| 99.999% (five nines)     | 5.25 minutes       | 25.9 seconds       | 6.05 seconds       |
| 99.9999% (six nines)     | 31.56 seconds      | 2.59 seconds       | 604.8 milliseconds |
| 99.99999% (seven nines)  | 3.15 seconds       | 263 milliseconds   | 60.5 milliseconds  |
| 99.999999% (eight nines) | 315.6 milliseconds | 26.3 milliseconds  | 6 milliseconds     |
| 99.9999999% (nine nines) | 31.6 milliseconds  | 2.6 milliseconds   | 0.6 milliseconds   |

### Extra jargon
- **Reliability:** How consistently the system functiosn without failure over time. It answers the question: "Will the system work properly without breaking down?".
- **Availability:** The percentage of time a system is up and accessible. It answers the question: "Can I use this system right not?"

High availability just minimizes downtime, maybe with an active-passive setup. Whilst fault tolerance would mean zero downtime, so like an active-active system.

A lot of this is kind of jargon, so I wouldn't focus on it too much.