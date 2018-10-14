        case 0xCB37:
            swap(cycles,reg.F,reg.A);
            break;
        case 0xCB30:
            swap(cycles,reg.F,reg.B);
            break;
        case 0xCB31:
            swap(cycles,reg.F,reg.C);
            break;
        case 0xCB32:
            swap(cycles,reg.F,reg.D);
            break;
        case 0xCB33:
            swap(cycles,reg.F,reg.E);
            break;
        case 0xCB34:
            swap(cycles,reg.F,reg.H);
            break;
        case 0xCB35:
            swap(cycles,reg.F,reg.L);
            break;
        case 0xCB36:
            temp = read_byte(reg.HL);
            swap(cycles,reg.F,temp);
            cycles = 16;
            break;
        case 0xCB07:
            RLCA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB00:
            RLCA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB01:
            RLCA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB02:
            RLCA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB03:
            RLCA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB04:
            RLCA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB05:
            RLCA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB06:
            temp = read_byte(reg.HL);
            RLCA(cycles,reg.F,temp);
            cycles = 16;
            break;
         case 0xCB17:
            RLA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB10:
            RLA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB11:
            RLA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB12:
            RLA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB13:
            RLA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB14:
            RLA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB15:
            RLA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB16:
            temp = read_byte(reg.HL);
            RLA(cycles,reg.F,temp);
            cycles = 16;
            break;
         case 0xCB0F:
            RRCA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB08:
            RRCA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB09:
            RRCA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB0A:
            RRCA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB0B:
            RRCA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB0C:
            RRCA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB0D:
            RRCA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB0E:
            temp = read_byte(reg.HL);
            RRCA(cycles,reg.F,temp);
            cycles = 16;
            break;
         case 0xCB1F:
            RRA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB18:
            RRA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB19:
            RRA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB1A:
            RRA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB1B:
            RRA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB1C:
            RRA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB1D:
            RRA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB1E:
            temp = read_byte(reg.HL);
            RRA(cycles,reg.F,temp);
            cycles = 16;
            break;
    //Shifts
          case 0xCB27:
            SLA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB20:
            SLA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB21:
            SLA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB22:
            SLA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB23:
            SLA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB24:
            SLA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB25:
            SLA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB26:
            temp = read_byte(reg.HL);
            SLA(cycles,reg.F,temp);
            cycles = 16;
            break;   
          case 0xCB2F:
            SRL(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB28:
            SRL(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB29:
            SRL(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB2A:
            SRL(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB2B:
            SRL(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB2C:
            SRL(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB2D:
            SRL(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB2E:
            temp = read_byte(reg.HL);
            SRL(cycles,reg.F,temp);
            cycles = 16;
            break;   
          case 0xCB3F:
            SRL(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB38:
            SRL(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB39:
            SRL(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB3A:
            SRL(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB3B:
            SRL(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB3C:
            SRL(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB3D:
            SRL(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB3E:
            temp = read_byte(reg.HL);
            SRL(cycles,reg.F,temp);
            cycles = 16;
            break;  
         case 0xCB47:
            cmpbit_b_r(cycles,reg.F,reg.A,0);
            break;
         case 0xCB40:
            cmpbit_b_r(cycles,reg.F,reg.B,0);
            break;
        case 0xCB41:
            cmpbit_b_r(cycles,reg.F,reg.C,0);
            break;
          case 0xCB42:
            cmpbit_b_r(cycles,reg.F,reg.D,0);
            break;
        case 0xCB43:
            cmpbit_b_r(cycles,reg.F,reg.E,0);
            break;  
         case 0xCB44:
            cmpbit_b_r(cycles,reg.F,reg.H,0);
            break;
        case 0xCB45:
            cmpbit_b_r(cycles,reg.F,reg.L,0);
            break;
        case 0xCB46:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,0);
            cycles = 16;
            break;
         case 0xCB4F:
            cmpbit_b_r(cycles,reg.F,reg.A,1);
            break;
         case 0xCB48:
            cmpbit_b_r(cycles,reg.F,reg.B,1);
            break;
        case 0xCB49:
            cmpbit_b_r(cycles,reg.F,reg.C,1);
            break;
        case 0xCB4A:
            cmpbit_b_r(cycles,reg.F,reg.D,1);
            break;
        case 0xCB4B:
            cmpbit_b_r(cycles,reg.F,reg.E,1);
            break;  
         case 0xCB4C:
            cmpbit_b_r(cycles,reg.F,reg.H,1);
            break;
        case 0xCB4D:
            cmpbit_b_r(cycles,reg.F,reg.L,1);
            break;
        case 0xCB4E:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,1);
            cycles = 16;
            break;
         case 0xCB57:
            cmpbit_b_r(cycles,reg.F,reg.A,2);
            break;
         case 0xCB50:
            cmpbit_b_r(cycles,reg.F,reg.B,2);
            break;
        case 0xCB51:
            cmpbit_b_r(cycles,reg.F,reg.C,2);
            break;
        case 0xCB52:
            cmpbit_b_r(cycles,reg.F,reg.D,2);
            break;
        case 0xCB53:
            cmpbit_b_r(cycles,reg.F,reg.E,2);
            break;  
         case 0xCB54:
            cmpbit_b_r(cycles,reg.F,reg.H,2);
            break;
        case 0xCB55:
            cmpbit_b_r(cycles,reg.F,reg.L,2);
            break;
        case 0xCB56:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,2);
            cycles = 16;
            break;
         case 0xCB5F:
            cmpbit_b_r(cycles,reg.F,reg.A,3);
            break;
         case 0xCB58:
            cmpbit_b_r(cycles,reg.F,reg.B,3);
            break;
        case 0xCB59:
            cmpbit_b_r(cycles,reg.F,reg.C,3);
            break;
        case 0xCB5A:
            cmpbit_b_r(cycles,reg.F,reg.D,3);
            break;
        case 0xCB5B:
            cmpbit_b_r(cycles,reg.F,reg.E,3);
            break;  
         case 0xCB5C:
            cmpbit_b_r(cycles,reg.F,reg.H,3);
            break;
        case 0xCB5D:
            cmpbit_b_r(cycles,reg.F,reg.L,3);
            break;
        case 0xCB5E:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,3);
            cycles = 16;
            break;
	    
      case 0xCBC7:
            set_b_r(cycles,reg.A,reg.pc++);
            break;
         case 0xCBC0:
            set_b_r(cycles,reg.B,reg.pc++);
            break;
        case 0xCBC1:
            set_b_r(cycles,reg.C,reg.pc++);
            break;
          case 0xCBC2:
            set_b_r(cycles,reg.D,reg.pc++);
            break;
        case 0xCBC3:
            set_b_r(cycles,reg.E,reg.pc++);
            break;  
         case 0xCBC4:
            set_b_r(cycles,reg.H,reg.pc++);
            break;
        case 0xCBC5:
            set_b_r(cycles,reg.L,reg.pc++);
            break;
        case 0xCBC6:
            temp = read_byte(reg.HL);
            set_b_r(cycles,temp,reg.pc++);
            cycles = 16;
            break;
        case 0xCB87:
            res_b_r(cycles,reg.A,reg.pc++);
            break;
         case 0xCB80:
            res_b_r(cycles,reg.B,reg.pc++);
            break;
        case 0xCB81:
            res_b_r(cycles,reg.C,reg.pc++);
            break;
          case 0xCB82:
            res_b_r(cycles,reg.D,reg.pc++);
            break;
        case 0xCB83:
            res_b_r(cycles,reg.E,reg.pc++);
            break;  
         case 0xCB84:
            res_b_r(cycles,reg.H,reg.pc++);
            break;
        case 0xCB85:
            res_b_r(cycles,reg.L,reg.pc++);
            break;
        case 0xCB86:
            temp = read_byte(reg.HL);
            res_b_r(cycles,temp,reg.pc++);
            cycles = 16;
            break;       
