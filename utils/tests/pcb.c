#include <stdio.h>
#include <stdbool.h>
#include <pcb/pcb.h>
#include <stdlib.h>
#include <commons/string.h>
#include <cspecs/cspec.h>

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
        it("Se serializa y se deserializa una pcb sin perdida de información para instruccion NORMAL") {
            t_pcb* pcb = malloc(sizeof(t_pcb));
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
            pcb->pid = 2;
            pcb->tamanio = 20;
            pcb->programCounter = 5;
            pcb->prioridad = 2;
            pcb->nom_arch_inst = string_duplicate("archivo.pdf");
            pcb->estado = BLOCKED;
            pcb->registers = registers;
            pcb->params = NULL;
            pcb->instruccion = NORMAL;
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
            should_int(deserialized_pcb->instruccion) be equal to (NORMAL);
            should_ptr(deserialized_pcb->params) be equal to (NULL);
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
        it("Se serializa y se deserializa una pcb sin perdida de información para instruccion FINISH") {
            t_pcb* pcb = malloc(sizeof(t_pcb));
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
            pcb->pid = 2;
            pcb->tamanio = 20;
            pcb->params = NULL;
            pcb->instruccion = FINISH;
            pcb->programCounter = 5;
            pcb->prioridad = 2;
            pcb->nom_arch_inst = string_duplicate("archivo.pdf");
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
            should_int(deserialized_pcb->instruccion) be equal to (FINISH);
            should_ptr(deserialized_pcb->params) be equal to (NULL);
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
        it("Se serializa y se deserializa una pcb sin perdida de información para instruccion WAIT") {
            t_pcb* pcb = malloc(sizeof(t_pcb));
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
            pcb->pid = 2;
            pcb->tamanio = 20;
            pcb->instruccion = WAIT;
            pcb->params = string_duplicate("AR");
            pcb->programCounter = 5;
            pcb->prioridad = 2;
            pcb->nom_arch_inst = string_duplicate("archivo.pdf");
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
            should_int(deserialized_pcb->instruccion) be equal to (WAIT);
            should_string(deserialized_pcb->params) be equal to ("AR");
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
        it("Se serializa y se deserializa una pcb sin perdida de información para instruccion SIGNAL") {
            t_pcb* pcb = malloc(sizeof(t_pcb));
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
            pcb->pid = 2;
            pcb->tamanio = 20;
            pcb->instruccion = SIGNAL;
            pcb->params = string_duplicate("RA");
            pcb->programCounter = 5;
            pcb->prioridad = 2;
            pcb->nom_arch_inst = string_duplicate("archivo.pdf");
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
            should_int(deserialized_pcb->instruccion) be equal to (SIGNAL);
            should_string(deserialized_pcb->params) be equal to ("RA");
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
        it("Se serializa y se deserializa una pcb sin perdida de información para instruccion SLEEP") {
            t_pcb* pcb = malloc(sizeof(t_pcb));
            t_reg registers;
            registers.AX = 0;
            registers.BX = 2;
            registers.CX = 3;
            registers.DX = 20;
            pcb->pid = 2;
            pcb->tamanio = 20;
            pcb->instruccion = SLEEP;
            pcb->params = 3;
            pcb->programCounter = 5;
            pcb->prioridad = 2;
            pcb->nom_arch_inst = string_duplicate("archivo.pdf");
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
            should_int(deserialized_pcb->instruccion) be equal to (SLEEP);
            should_int(deserialized_pcb->params) be equal to (3);
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
        it("Usando crear_pcb") {
            t_pcb* pcb = crear_pcb(1, string_duplicate("nombre"), 2, 3);
        	void* serialized_pcb = serialize_pcb(pcb);
            t_pcb* deserialized_pcb = deserialize_pcb(serialized_pcb);
            should_int(deserialized_pcb->pid) be equal to (1);
            should_int(deserialized_pcb->tamanio) be equal to (2);
            should_int(deserialized_pcb->programCounter) be equal to (0);
            should_int(deserialized_pcb->prioridad) be equal to (3);
            should_string(deserialized_pcb->nom_arch_inst) be equal to ("nombre");
            should_int(deserialized_pcb->estado) be equal to (NEW);
            should_int(deserialized_pcb->registers.AX) be equal to (0);
            should_int(deserialized_pcb->registers.BX) be equal to (0);
            should_int(deserialized_pcb->registers.CX) be equal to (0);
            should_int(deserialized_pcb->registers.DX) be equal to (0);
            should_int(deserialized_pcb->instruccion) be equal to (NORMAL);
            should_ptr(deserialized_pcb->params) be equal to (NULL);
            destroy_pcb(pcb);
            destroy_pcb(deserialized_pcb);
        } end
    } end
}