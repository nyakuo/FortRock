import java.io.*;

public class LCM {
    public int lcm(int I, int J) {
        int IR1, IR2, IR, ret_lcm;

        if(I < J) {
            IR1 = J;
            IR2 = I;
        }
        else {
            IR1 = I;
            IR2 = J;
        }

        IR = IR1 - (IR1/IR2) * IR2;

        while(IR>0) {
            IR1 = IR2;
            IR2 = IR;
            IR = IR1 - (IR1/IR2) * IR2;
        }

        ret_lcm = I*J/IR2;

        return ret_lcm;
    }
}
