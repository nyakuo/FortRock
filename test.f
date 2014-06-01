C**************************************************
C 最小公倍数
C**************************************************
      SUBROUTINE LCM(ret_lcm)
      INTEGER I, J, IR1, IR2

      J = 20
      I = 70

      IF(I < J) THEN
         IR1 = J
         IR2 = I
      ELSE
         IR1 = I
         IR2 = J
      ENDIF

      IR = IR1 - (IR1/IR2) * IR2

      DO WHILE(IR>0)
         IR1 = IR2
         IR2 = IR
         IR = IR1 - (IR1/IR2) * IR2
      ENDDO

      ret_lcm = I*J/IR2

      RETURN
      END
