#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    int   version         = 1;
    float successRate     = 0.05f;
    float minFartDistance = 1.0f;
    float maxFartDistance = 3.0f;
    int   particleCount   = 5;
};

#endif // CONFIG_H
