#include <base_component.h>
#include <queue_lib.h>
#include <events.h>
#include <common_defines.h>
#include <timer.h>





static void (*request_handlers[MAX_REQUEST_HANDLERS])(uint32_t*) = { NULL };

void base_component_init(){

}
void base_component_exit(){

}


// Register request handlers so event handler can call appropriate request handler
// according to id.
void base_component_request_register(uint32_t id, void (*handler)(uint32_t* req)){
    if (id >= MAX_REQUEST_HANDLERS) {
        printf("Error: ID %d is out of range.\n", id);
        return;
    }
    request_handlers[id] = handler;
    // printf("Registered handler for ID %d.\n", id);

}

void base_component_request_unregister(uint32_t id){
    if (id >= MAX_REQUEST_HANDLERS) {
        printf("Error: ID %d is out of range.\n", id);
        return;
    }
    request_handlers[id] = NULL;
    // printf("Unregistered handler for ID %d.\n", id);

}



//below 2 functions are internal to the library
void base_component_component_installer(request_t* req){
    // Define a component install structure
    // Component install would include:
    //
    // (1) Setting the payload of request to be able to get the event handler function pointer
    // and the request handler function pointer
    // 
    // (2) adding the component event handler
    // using the: int register_event_handler(uint32_t id, uint32_t bitmask, event_handler_t handler) function
    // 
    // (3) Adding the component request handler using the void base_component_request_register(uint32_t id, void (*handler)(uint32_t* req)){
    // function
    request_comp_installer_t* req_comp_install = req->payload;

    register_event_handler(req_comp_install->id, req_comp_install->bitmask, req_comp_install->event_handler);

    base_component_request_register(req_comp_install->id, req_comp_install->request_handler);
    // printf("Successfully installed new component\n");
}

void base_component_component_uninstaller(request_t* req){
    // Define a component uninstall structure
    // Component uninstall would include:
    //
    // (1) Setting the payload of request to be able to get the event handler function pointer
    // and the request handler function pointer
    // 
    // (2) Removing the component event handler using the ________ function
    // 
    // 
    // (3) Removing the component request handler using the _________
    // function

    request_comp_installer_t* req_comp_install = req->payload;

    unregister_event_handler(req_comp_install->id);

    base_component_request_unregister(req_comp_install->id);
}


// The base event handler should run when new data is written to the request queue.
// (1) It should get the payload 
// (2) It should check if payload is for its own. If not proceed further
// (3) It should determine for which other component is payload intended for. 
// (4) Then it should pass the payload to the intended component
void base_component_event_handler(uint32_t bitmap){
    uint32_t req_size = 0;
    request_t* req = get_request(&req_size);  // Get the request (req contains the ID)

    if (req == NULL || req_size == 0) {
        printf("No request found or invalid request size.\n");
        return;
    }

    uint32_t id = req->req_id;  // Assuming the first element in the req array is the ID

    if (id >= MAX_REQUEST_HANDLERS) {
        printf("Error: Invalid request ID %d.\n", id);
        return;
    }

    if (id == REQUEST_INSTALL_ID){
        base_component_component_installer(req);
    } else if (id == REQUEST_UNINSTALL_ID){
        base_component_component_uninstaller(req);
    } else if (request_handlers[id] != NULL) {
        // Call the corresponding request handler, passing the req pointer  
        request_handlers[id](req);
        
    } else {
        printf("No handler registered for ID %d.\n", id);
        return;
    }
    // req->consumed=1;
    get_request_pop();
}


