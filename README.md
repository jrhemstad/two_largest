# Two Largest Elements

This was an algorithmic problem first discussed in [ADSP Episode 72: C++ Algorithm Family Feud!](https://adspthepodcast.com/2022/04/08/Episode-72.html). Then Tyler Weaving did some [initial profiling that he shared on Twitter](https://twitter.com/squirrel428_/status/1516056776004800515) which then [Conor responded to](https://twitter.com/adspthepodcast/status/1516097211762888710?s=20&t=7CLPpFoMnDwta3U4eDj3QA). Conor later wrote some initial and most likely totally **incorrect** profiling code that yielded the graph below. This repo will set up some benchmarks that profile these algorithms correctly.

![image](https://user-images.githubusercontent.com/36027403/164339315-8e2bd97b-b388-4148-904b-25a0cce7e4af.png)


# Locking CPU Clocks

CPUs are constantly changing their voltage and frequency in response to load and temperature. 
This means how "fast" your CPU is can change from moment to moment and may be different in the winter vs the summer!
While this is useful for day to day usage, it is harmful when trying to gather deterministic and reproducible performance numbers.

In order to generate the most reliable numbers possible, one should disable CPU frequency scaling.

## Install `cpupower`

On Ubunutu, install the `cpupower` utility:

```
sudo apt-get install -y linux-tools-$(uname -r)
```

For more information or instructions for other distros, see https://wiki.archlinux.org/title/CPU_frequency_scaling