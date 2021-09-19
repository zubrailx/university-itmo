package com.company.interfaces;

import com.company.human.AdvancedHuman;

public interface Movable {

    void run(boolean b);
    void runToHuman(AdvancedHuman human);


    void comeUp();
    void comeUp(AdvancedHuman human);

    void takeStepsHow(String adjective, int n);
}
