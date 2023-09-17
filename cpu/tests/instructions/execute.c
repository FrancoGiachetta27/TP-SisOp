#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include <instructions/execute.h>

context (example) {
    describe("Testing Execute") {
        it("SET instruction") {
            char **inst = string_split("SET AX 55", " ");
            t_reg reg;
            execute(&reg, inst, NULL);
            should_int(reg.AX) be equal to (55);
        } end
        it("SUM instruction") {
            char** inst = string_split("SUM AX BX", " ");
            t_reg reg;
            reg.AX = 20;
            reg.BX = 10;
            execute(&reg, inst, NULL);
            should_int(reg.BX) be equal to (10);
            should_int(reg.AX) be equal to (30);
        } end
        it("SUB instruction") {
            char** inst = string_split("SUB AX BX", " ");
            t_reg reg;
            reg.AX = 50;
            reg.BX = 10;
            execute(&reg, inst, NULL);
            should_int(reg.BX) be equal to (10);
            should_int(reg.AX) be equal to (40);
        } end
    } end
    describe("Testing Instruccions") {
        it("Set value in AX Register") {
            t_reg reg;
            set(&reg, "AX", "20");
            should_int(reg.AX) be equal to (20);
        } end
        it("Set value in BX Register") {
            t_reg reg;
            set(&reg, "BX", "5");
            should_int(reg.BX) be equal to (5);
        } end
        it("Set value in CX Register") {
            t_reg reg;
            set(&reg, "CX", "100");
            should_int(reg.CX) be equal to (100);
        } end
        it("Set value in DX Register") {
            t_reg reg;
            set(&reg, "DX", "67");
            should_int(reg.DX) be equal to (67);
        } end
        it("Set value in register and reset it") {
            t_reg reg;
            set(&reg, "AX", "20");
            set(&reg, "AX", "10");
            should_int(reg.AX) be equal to (10);
        } end
        it("Set value in two registers and sum them") {
            t_reg reg;
            reg.AX = 20;
            reg.BX = 10;
            sum(&reg, "AX", "BX");
            should_int(reg.AX) be equal to (30);
            should_int(reg.BX) be equal to (10);
        } end
        it("Set value in two registers and sub them") {
            t_reg reg;
            reg.AX = 20;
            reg.BX = 10;
            sub(&reg, "AX", "BX");
            should_int(reg.AX) be equal to (10);
            should_int(reg.BX) be equal to (10);
        } end
    } end
}