#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

struct HealthComponent {
    int maxHealthPercentage;
    int curHealthPercentage;

    HealthComponent(int maxHealthPercentage = 100) {
        this->maxHealthPercentage = maxHealthPercentage;
        curHealthPercentage = maxHealthPercentage;
    }
};

#endif