// DO NOT INCLUDE ANY OTHER LIBRARIES/FILES
#include "pointer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// In this assignment, you can assume that function parameters are valid and the memory is managed by the caller //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the area of a rectangle
// The shape is guaranteed to be a valid rectangle

double rectangle_area(void* shape) {
  rectangle_t* rectangle = (rectangle_t*)shape;
  return rectangle->width * rectangle->length;
}

// Returns the area of an equilateral triangle
// The shape is guaranteed to be a valid triangle
// The area of an equilateral triangle is sqrt(3)/4 times length squared
double triangle_area(void* shape)
{
    triangle_t* triangle = (triangle_t*)shape;
    return (triangle->length * triangle->length) * (sqrt(3)/4);
}

// Returns the perimeter of a rectangle
// The shape is guaranteed to be a valid rectangle
double rectangle_perimeter(void* shape)
{
    rectangle_t* rectangle = (rectangle_t*)shape;
    return 2 * (rectangle->length + rectangle->width);
}

// Returns the perimeter of an equilateral triangle
// The shape is guaranteed to be a valid triangle
double triangle_perimeter(void* shape)
{
    triangle_t* triangle = (triangle_t*)shape;
    return 3 * triangle->length;
}

// Initializes a rectangle shape
void rectangle_construct(rectangle_t* shape, const char* name, double width, double length)
{    
    shape->shape.name = name;
    shape->shape.edges = 4; 
    shape->shape.virtual_func_table.area = rectangle_area; 
    shape->shape.virtual_func_table.perimeter = rectangle_perimeter; 
    shape->width = width;
    shape->length = length;
}

// Initializes a triangle shape
void triangle_construct(triangle_t* shape, const char* name, double length)
{
    shape->shape.name = name;
    shape->shape.edges = 3; 
    shape->shape.virtual_func_table.area = triangle_area; 
    shape->shape.virtual_func_table.perimeter = triangle_perimeter;
    shape->length = length;
}

// Compares the area of shape1 with shape2
// Returns -1 if the area of shape1 is less than the area of shape2
// Returns 1 if the area of shape1 is greater than the area of shape2
// Returns 0 if the area of shape1 is equal to the area of shape2
int compare_by_area(shape_t* shape1, shape_t* shape2)
{
    
    double area1 = shape1->virtual_func_table.area(shape1);
    double area2 = shape2->virtual_func_table.area(shape2);
    
    if (area1 < area2) {
        return -1;
    } else if (area1 > area2) {
        return 1;
    } else {
        return 0;
    }
    
    return 0;
}

// Compares the perimeter of shape1 with shape2
// Returns -1 if the perimeter of shape1 is less than the perimeter of shape2
// Returns 1 if the perimeter of shape1 is greater than the perimeter of shape2
// Returns 0 if the perimeter of shape1 is equal to the perimeter of shape2
int compare_by_perimeter(shape_t* shape1, shape_t* shape2)
{
    double peri1 = shape1->virtual_func_table.perimeter(shape1);
    double peri2 = shape2->virtual_func_table.perimeter(shape2);

    if (peri1 < peri2) {
        return -1;
    } else if (peri1 > peri2) {
        return 1;
    } else {
        return 0;
    }

    return 0;
}

//
// Linked list functions
//

// Initializes a singly linked list
// If compare is NULL, the list is unsorted and new nodes are inserted at the head of the list
// If compare is not NULL, the list is sorted in increasing order based on the comparison function
void linked_list_init(linked_list_t* list, compare_fn compare)
{
    list->head = NULL;
    list->compare = compare;
}

// Inserts a node into the linked list based on the list comparison function
void linked_list_insert(linked_list_t* list, linked_list_node_t* node)
{
    if (list->compare == NULL || list->head == NULL || list->compare(node->shape, list->head->shape) <= 0) {
        node->next = list->head;
        list->head = node;
    } else {
        linked_list_node_t* current = list->head;
        while (current->next != NULL && list->compare(node->shape, current->next->shape) > 0) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
}

// Removes all nodes from the linked list containing the given shape
void linked_list_remove(linked_list_t* list, shape_t* shape)
{
    linked_list_node_t* current = list->head;
    linked_list_node_t* prev = NULL;
    while (current != NULL) {
        if (current->shape == shape) {
            if (prev == NULL) {
                list->head = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
}


//
// Tree iterator functions
//

// Initializes an iterator to the beginning of a tree (i.e., first in-order node)
void tree_iterator_begin(tree_iterator_t* iter, tree_node_t* root)
{
    iter->curr = root;
    iter->depth = 0;
    if (root != NULL) {
        while (iter->curr->left != NULL) {
            if (iter->depth >= MAX_DEPTH) {
                return;
            }
            iter->parents[iter->depth++] = iter->curr;
            iter->curr = iter->curr->left;
        }
    }
}

// Updates an iterator to move to the next in-order node in the tree if possible
// Moving past the last in-order node in the tree results in the iterator reaching a NULL state representing the end of the tree
void tree_iterator_next(tree_iterator_t* iter)
{
    if (iter->curr == NULL) {
        return;
    }

    if (iter->curr->right != NULL) {
        iter->curr = iter->curr->right;
        while (iter->curr->left != NULL) {
            if (iter->depth >= MAX_DEPTH) {
                return;
            }
            iter->parents[iter->depth++] = iter->curr;
            iter->curr = iter->curr->left;
        }
    } else {
        while (iter->depth > 0 && iter->parents[iter->depth - 1]->right == iter->curr) {
            iter->curr = iter->parents[--iter->depth];
        }
        iter->curr = iter->depth > 0 ? iter->parents[--iter->depth] : NULL;
    }
}

// Returns true if iterator is at the end of the tree or false otherwise
// The end of the tree is the position after the last in-order node in the tree (i.e., NULL state from tree_iterator_next)
bool tree_iterator_at_end(tree_iterator_t* iter)
{
    return iter->curr == NULL;
}

// Returns the current node that the iterator references or NULL if the iterator is at the end of the list
tree_node_t* tree_iterator_get_node(tree_iterator_t* iter)
{
    return iter->curr;
}

// Returns the current shape that the iterator references or NULL if the iterator is at the end of the list
shape_t* tree_iterator_get_shape(tree_iterator_t* iter)
{
    return iter->curr != NULL ? iter->curr->shape : NULL;
}

//
// Tree analysis functions
//

// Returns the maximum, minimum, and average area of shapes in the tree
// An empty tree should not modify the maximum, minimum, or average
void max_min_avg_area(tree_node_t* root, double* max, double* min, double* avg)
{
    if (root == NULL) {
        return;
    }
    tree_iterator_t iter;
    tree_iterator_begin(&iter, root);

    double area = shape_area(iter.curr->shape);
    *max = area;
    *min = area;
    double sum = area;
    int count = 1;

    while (!tree_iterator_at_end(&iter)) {
        tree_iterator_next(&iter);
        if (!tree_iterator_at_end(&iter)) {
            area = shape_area(tree_iterator_get_shape(&iter));
            if (area > *max) *max = area;
            if (area < *min) *min = area;
            sum += area;
            count++;
        }
    }
    *avg = sum / count;
}

// Executes the func function for each node in the tree in-order
// The function takes in an input data and returns an output data, which is used as input to the next call to the function
// The initial input data is provided as a parameter to foreach, and foreach returns the final output data
// For example, if there are three nodes, foreach should behave like: return func(node3, func(node2, func(node1, data)))
double foreach(tree_node_t* root, foreach_fn func, double data)
{
    if (root == NULL) {
        return data;
    }
    data = foreach(root->left, func, data);
    data = func(root->shape, data);
    data = foreach(root->right, func, data);
    return data;
}
