#ifndef SH_TEMPLATE_H
#define SH_TEMPLATE_H



/**
* @file
* 
* @brief `shtemplate` header file.
*
*
* @author mrsinho <https://github.com/mrsinho>
*/



#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



/**
* @struct shtemplate
* @brief Empty struct. It means nothing.
*/
typedef struct shtemplate {
    float empty; /**< Empty float, it means nothing. */
} shtemplate;



/**
 * @brief A template function
 * 
 * @param void [no arguments]
 * 
 * @return status of the function, equal to 1 on success.
*/
int shTemplateFunction(
    void
);



#ifdef __cplusplus
}
#endif//_cplusplus

#endif//SH_TEMPLATE_H