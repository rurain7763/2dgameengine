#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

struct HealthComponent {
    int healthPercentage;

    HealthComponent(int healthPercentage = 100) {
        this->healthPercentage = healthPercentage;
    }
};

#endif