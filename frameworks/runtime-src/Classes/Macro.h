#ifndef __Boids_Macro_h_
#define __Boids_Macro_h_

#define BOIDS_SYNTHESIZE_READONLY(varType, varName)\
private: varType varName; \
public: virtual varType get##varName(void) const { return varName; }

#define BOIDS_SYNTHESIZE_WRITEONLY(varType, varName)\
private: varType varName; \
public: virtual void set##varName(varType var){ varName = var; }

#define BOIDS_SYNTHESIZE(varType, varName)\
private: varType varName; \
public: virtual varType get##varName(void) const { return varName; }\
public: virtual void set##varName(varType var){ varName = var; }

#endif