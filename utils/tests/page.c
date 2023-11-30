#include <stdio.h>
#include <stdbool.h>
#include <page/page.h>
#include <stdlib.h>
#include <commons/string.h>
#include <cspecs/cspec.h>

context (page) {
    describe("Testing Page") {
        it("Se serializa y se deserializa una pagina sin perdida de información") {
            t_pag* page = malloc(sizeof(*page));
            page->displacement = 0;
            page->page_number=2;
            page->pid=3;
        	void* serialized_page = serialize_page(page);
            t_pag* deserialized_page = deserialize_page(page);
            should_int(deserialized_page->displacement) be equal to (0);
            should_int(deserialized_page->page_number) be equal to (2);
            should_int(deserialized_page->pid) be equal to (3);
            free(page);
            free(deserialized_page);
            free(serialized_page);
        } end
        it("Se serializa y se deserializa una pagina para el swap sin perdida de información") {
            t_pag_swap* page = malloc(sizeof(*page));
            page->swap_block= 0;
            page->page_info_size=32;
            page->page_content="00000000000000000000000000000000";
        	void* serialized_page = serialize_page_for_swap(page);
            t_pag_swap* deserialized_page = deserialize_page_for_swap(page);
            should_int(deserialized_page->swap_block) be equal to (0);
            should_int(deserialized_page->page_info_size) be equal to (32);
            should_int(deserialized_page->page_content) be equal to ("00000000000000000000000000000000");
            free(page);
        } end
    } end
}