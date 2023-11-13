#include "page.h"

int size_of_page() {
	return sizeof(uint32_t) + 2 * sizeof(int);
}

int size_of_page_for_mov_out() {
	return 2 * sizeof(uint32_t) + 2 * sizeof(int);
}

t_pag* page_new(uint32_t pid, int page_number, int displacement) {
	t_pag* page = malloc(sizeof(*page));
	page->pid = pid;
	page->page_number = page_number;
	page->displacement = displacement;
	return page;
}

t_mov_out* page_for_mov_out(uint32_t pid, int page_number, int displacement, uint32_t register_value) {
	t_mov_out* page = malloc(sizeof(*page));
	page->pid = pid;
	page->page_number = page_number;
	page->displacement = displacement;
	page->register_value = register_value;
	return page;
}

void* serialize_page_for_mov_out(t_mov_out* page) {
	void* buffer = malloc(size_of_page_for_mov_out());
	int offset = 0;
	memcpy(buffer + offset, &page->page_number, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, &page->displacement, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, &page->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &page->register_value, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return buffer;
}

void* serialize_page(t_pag* page) {
	void* buffer = malloc(size_of_page());
	int offset = 0;
	memcpy(buffer + offset, &page->page_number, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, &page->displacement, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, &page->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return buffer;
}

t_pag* deserialize_page(void* buffer) {
	t_pag* page = malloc(sizeof(*page));
	int offset = 0;
	memcpy(&page->page_number, buffer + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&page->displacement, buffer + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&page->pid, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return page;
}

t_mov_out* deserialize_page_for_mov_out(void* buffer) {
	t_mov_out* page = malloc(sizeof(*page));
	int offset = 0;
	memcpy(&page->page_number, buffer + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&page->displacement, buffer + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&page->pid, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&page->register_value, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return page;
}

void send_page_for_mov_out(int op_code, t_mov_out* page, int client_socket, t_log* logger) {
	t_package* package = create_empty_package(op_code);
    package->size = size_of_page_for_mov_out();
    package->buffer = serialize_page_for_mov_out(page);
    send_package(package, client_socket, logger);
}

void send_page(int op_code, t_pag* page, int client_socket, t_log* logger) {
	t_package* package = create_empty_package(op_code);
    package->size = size_of_page();
    package->buffer = serialize_page(page);
    send_package(package, client_socket, logger);
}

t_pag* receive_page(int client_socket, t_log* logger) {
	void* buffer = receive_buffer(client_socket, logger);
   	return deserialize_page(buffer);
}

t_mov_out* receive_page_for_mov_out(int client_socket, t_log* logger) {
	void* buffer = receive_buffer(client_socket, logger);
   	return deserialize_page_for_mov_out(buffer);
}