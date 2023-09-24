#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include <pcb/pcb.h>
#include <stdlib.h>

context (pcb) {
    describe("Testing PCB") {
        it("Se serializa y se deserializa los registros sin perdida de información") {
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
        	void* serialized_registers = serialize_registers(registers);
            t_reg deserialized_registers = deserialize_registers(serialized_registers);
            should_int(deserialized_registers.AX) be equal to (0);
            should_int(deserialized_registers.BX) be equal to (2);
            should_int(deserialized_registers.CX) be equal to (3);
            should_int(deserialized_registers.DX) be equal to (20);
            free(serialized_registers);
        } end
        it("Se serializa y se deserializa una pcb sin perdida de información") {
            t_pcb* pcb = malloc(sizeof(t_pcb*));
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
            pcb->pid = 2;
            pcb->tamanio = 20;
            pcb->programCounter = 5;
            pcb->prioridad = 2;
            pcb->nom_arch_inst = "archivo.pdf";
            pcb->estado = BLOCKED;
            pcb->registers = registers;

        	void* serialized_pcb = serialize_pcb(pcb);
            t_pcb* deserialized_pcb = deserialize_pcb(serialized_pcb);
            should_int(deserialized_pcb->pid) be equal to (2);
            should_int(deserialized_pcb->tamanio) be equal to (20);
            should_int(deserialized_pcb->programCounter) be equal to (5);
            should_int(deserialized_pcb->prioridad) be equal to (2);
            should_string(deserialized_pcb->nom_arch_inst) be equal to ("archivo.pdf");
            should_int(deserialized_pcb->estado) be equal to (BLOCKED);
            should_int(deserialized_pcb->registers.AX) be equal to (0);
            should_int(deserialized_pcb->registers.BX) be equal to (2);
            should_int(deserialized_pcb->registers.CX) be equal to (3);
            should_int(deserialized_pcb->registers.DX) be equal to (20);
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
    } end
}