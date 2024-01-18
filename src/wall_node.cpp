#include "wall_node.h"

#include "bsp_tree.h"
#include "common_struct.h"
#include "map.h"

Wall_Node* GetLast(Wall_Node* head) {
    if (head->next == NULL)
        return head;
    return GetLast(head->next);
}

Wall_Node* GetFirst(Wall_Node* head) {
    if (head->previous == NULL)
        return head;
    return GetFirst(head->previous);
}

// TODO: this may belong somewhere else, not sure where yet.
Wall_Node* FindOrder(BSP_Tree* bsp_tree, Map& map) {
    if (bsp_tree == NULL)
        return NULL;

    if (bsp_tree->front == NULL && bsp_tree->back == NULL) {
        Wall_Node* new_head = new Wall_Node;
        new_head->id = bsp_tree->id;
        new_head->next = NULL;
        new_head->previous = NULL;
        return new_head;
    }
    // Vertex to refer to camera position
    Vertex camera_vertex = {
        .id = -1,
        .x = 0.0f,
        .y = 0.0f
    };

    Wall* cur_wall = map.GetWallByID(bsp_tree->id);

    Wall_Node* cur_node = new Wall_Node;
    cur_node->id = bsp_tree->id;
    cur_node->next = NULL;
    cur_node->previous = NULL;
    
    Wall_Node* front = FindOrder(bsp_tree->front, map);
    Wall_Node* back = FindOrder(bsp_tree->back, map);
    
    // Store the value of is_front, since we'll reference it a lot.
    bool is_front_return = is_front(camera_vertex, cur_wall);
    
    if (is_front_return) {

        if (front != NULL) {
            // Connect front->...->last_of_front->cur
            Wall_Node* last_of_front = GetLast(front);
            cur_node->previous = last_of_front;
            last_of_front->next = cur_node;
        }

        if (back != NULL) {
            // Connect cur->back->...->last_of_back
            cur_node->next = back;
            back->previous = cur_node;
        }
        
        // Return the new head
        return GetFirst(cur_node);
    }
    else {
        if (back != NULL) {
            // Connect back->...->last_of_back->cur
            Wall_Node* last_of_back = GetLast(back);
            cur_node->previous = last_of_back;
            last_of_back->next = cur_node;
        }

        if (front != NULL) {
            // Connect cur->front->...last_of_front
            cur_node->next = front;
            front->previous = cur_node;
        }

        // Return the new head
        return GetFirst(cur_node);
    }
}