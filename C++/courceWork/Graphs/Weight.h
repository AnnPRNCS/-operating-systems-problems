//
// Created by hal4dan on 20.09.2019.
//

#ifndef COURCEWORK_WEIGHT_H
#define COURCEWORK_WEIGHT_H
class Weight {
private:
    int firstVertex;
    int lastVertex;
    int weight;
public:
    Weight() {
        firstVertex = 0;
        lastVertex=0;
        weight=0;
    }
    Weight(int firstVertex_, int lastVertex_, int weight_) : firstVertex(firstVertex_), lastVertex(lastVertex_),
                                                             weight(weight_) {

    };
    int getWeight() {
        return weight;
    }
    int getLastVertex() {
        return lastVertex;
    }
    int getFirstVertex() {
        return firstVertex;
    }
    void setWeight(int weight_) {
        weight = weight_;
    }
    void setLastVertex(int lastVertex_) {
        lastVertex = lastVertex_;
    }
    void setFirstVertex(int firstVertex_) {
        firstVertex = firstVertex_;
    }
};

#endif //COURCEWORK_WEIGHT_H
