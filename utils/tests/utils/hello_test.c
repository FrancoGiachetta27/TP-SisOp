#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include <utils/hello.h>

context (example) {
    describe("Testing Suma") {
        it("suma() de dos numeros deberia de dar el resultado de la suma") {
        	int resultado = suma(1,2);
            should_int(resultado) be equal to(3);
        } end
    } end
}
