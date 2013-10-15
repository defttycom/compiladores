#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_tree.h"
#include "iks_ast.h"
#include "iks_types.h"
#include "parser.h"

int verify_coercion(comp_tree_t *id, comp_tree_t *expr) {
  int ret=0;
  iks_ast_node_value_t *idn,*exprn;
  idn = id->item;
  exprn = expr->item;
  comp_grammar_symbol_t *ids,*exprs;
  ids = idn->symbol;
  exprs = exprn->symbol;
  if (exprs) { //gambi because call function has no symbol
    if((ids->iks_type == IKS_INT)&&(exprs->iks_type == IKS_BOOL)) {
      //printf("coercion int to bool\n");
      idn->need_coercion=IKS_COERCION_INT_TO_BOOL;
    }
    else if((ids->iks_type == IKS_INT)&&(exprs->iks_type == IKS_FLOAT)) {
      //printf("coercion int to float\n");
      idn->need_coercion=IKS_COERCION_INT_TO_FLOAT;
    }
    else if((ids->iks_type == IKS_FLOAT)&&(exprs->iks_type == IKS_BOOL)) {
      //printf("coercion float to bool\n");
      idn->need_coercion=IKS_COERCION_FLOAT_TO_BOOL;
    }
    else if((ids->iks_type == IKS_FLOAT)&&(exprs->iks_type == IKS_INT)) {
      //printf("coercion float to int\n");
      idn->need_coercion=IKS_COERCION_FLOAT_TO_INT;
    }
    else if((ids->iks_type == IKS_BOOL)&&(exprs->iks_type == IKS_INT)) {
      //printf("coercion bool to int\n");
      idn->need_coercion=IKS_COERCION_BOOL_TO_INT;
    }
    else if((ids->iks_type == IKS_BOOL)&&(exprs->iks_type == IKS_FLOAT)) {
      //printf("coercion bool to float\n");
      idn->need_coercion=IKS_COERCION_BOOL_TO_FLOAT;
    }
    else if((exprs->iks_type == IKS_CHAR)) {
      fprintf(stderr,"%d: identificador '%s' conversao impossivel do tipo char\n",exprs->code_line_number,exprs->value);
        ret=IKS_ERROR_CHAR_TO_X;
    }
    else if((ids->iks_type != IKS_STRING)&&(exprs->iks_type == IKS_STRING)) {
      fprintf(stderr,"%d: identificador '%s' conversao impossivel do tipo string\n",exprs->code_line_number,exprs->value);
        ret=IKS_ERROR_STRING_TO_X;
    }
    //else if ((ids->iks_type==IKS_INT) ||\
    //          (ids->iks_type==IKS_FLOAT) ||\
    //          (ids->iks_type==IKS_BOOL)) {
    else {
      fprintf(stderr,"identificador '%s' e '%s' de tipos incompativeis\n",ids->value,exprs->value);
        ret=IKS_ERROR_WRONG_TYPE;
    }
  }
  else {
    printf("expr is null at coercion\n");
  }
  return ret;
}

int verify_function_args(comp_grammar_symbol_t *s, comp_list_t *args) {
  int ret=0;
  comp_grammar_symbol_t *s1,*s2;
  comp_list_t *l1,*l2;
  int sl1,sl2,diff;
  l1 = s->params->next;
  l2 = args->next;
  sl1 = comp_list_size(s->params->next);
  sl2 = comp_list_size(args->next);
  diff = sl1-sl2;
  if (diff!=0) {
    if (sl1>sl2) {
      fprintf(stderr,"faltam %d argumentos em '%s'\n",diff,s->value);
      ret=IKS_ERROR_MISSING_ARGS;
    }
    else {
      fprintf(stderr,"sobram %d argumentos em '%s'\n",diff*-1,s->value);
      ret=IKS_ERROR_EXCESS_ARGS;
    }
  }
  else if (sl1!=0){
    do {
       s1 = l1->item;
       s2 = l2->item;
       if (s1->iks_type!=s2->iks_type) {
        fprintf(stderr,"tipos incompativeis entre '%s' e '%s'\n",s1->value,s2->value);
        ret=IKS_ERROR_EXCESS_ARGS;
        break;
       }
       l1 = l1->next;
       l2 = l2->next;
    } while(l1 != s->params);
  }
  return ret;
}

int symbol_is_iks_type(comp_grammar_symbol_t *s,int iks_type) {
  int ret=1;
  //printf("%d vs %d\n",s->iks_type,iks_type);
  if (!(s->iks_type==iks_type)) {
    ret=0;
  }
  return ret;
}

<<<<<<< Updated upstream
=======
int infer_type(comp_tree_t *type1, comp_tree_t *type2) {
	iks_ast_node_value_t *type1n = type1->item;
	iks_ast_node_value_t *type2n = type2->item;
	
	if(type1n->iks_type == type2n->iks_type)
		return type1n->iks_type;
		
	if(type1n->iks_type == IKS_FLOAT) {
		int coercion = verify_coercion(type1, type2);
		if(coercion)
			return coercion;
		
		return IKS_FLOAT;
	} else if(type2n->iks_type == IKS_FLOAT) {
		int coercion = verify_coercion(type2, type1);
		if(coercion)
			return coercion;
		
		return IKS_FLOAT;
	}
	
	if(type1n->iks_type == IKS_INT) {
		int coercion = verify_coercion(type1, type2);
		if(coercion)
			return coercion;
		
		return IKS_INT;
	} else if(type2n->iks_type == IKS_INT) {
		int coercion = verify_coercion(type2, type1);
		if(coercion)
			return coercion;
		
		return IKS_INT;
	}
}

int type_error(comp_grammar_symbol_t *s, int error_type) {
  int ret=0;
  switch(error_type) {
    case IKS_ERROR_USE:
      if (s->decl_type==IKS_DECL_VAR) {
        fprintf(stderr,"line %d: identificador '%s' deve ser usado como variavel\n",s->code_line_number,s->value);    
        ret=IKS_ERROR_VARIABLE;
      }   
      else if (s->decl_type==IKS_DECL_VECTOR) {
        fprintf(stderr,"line %d: identificador '%s' deve ser usado como vetor\n",s->code_line_number,s->value);    
        ret=IKS_ERROR_VECTOR;
      }   
      else if (s->decl_type==IKS_DECL_FUNCTION) {
        fprintf(stderr,"line %d: identificador '%s' deve ser usado como funcao\n",s->code_line_number,s->value);    
        ret=IKS_ERROR_FUNCTION;
      }   
      else {
        fprintf(stderr,"line %d: identificador '%s' ???????????\n",s->code_line_number,s->value);    
        ret=99999;

      }   
      break;
    
    case IKS_ERROR_WRONG_PAR_RETURN:
      fprintf(stderr,"parametro nao e compativel com expressao de retorno.\n");
      ret=IKS_ERROR_WRONG_PAR_RETURN;
      break;
    
    case IKS_ERROR_WRONG_PAR_INPUT:
      if(s != NULL)
        fprintf(stderr,"line %d: '%s' deve ser identificador\n",s->code_line_number,s->value);
      else fprintf(stderr,"parâmetro para input deve ser identificador\n");
      ret = IKS_ERROR_WRONG_PAR_INPUT;
      break;

		case IKS_ERROR_DECLARED:
			ret = IKS_ERROR_DECLARED;
			break;

		case IKS_ERROR_UNDECLARED:
			ret = IKS_ERROR_UNDECLARED;
			break;
  }

	//memory_cleaner();

  return ret;
}

void open_scope() {
  comp_dict_t *symbol_table_local;
  symbol_table_local = new_comp_dict();
  scope = comp_stack_push(scope,(void*)symbol_table_local);
}

void close_scope() {
  comp_dict_t *st = (comp_dict_t*) comp_stack_top(scope);
  //comp_dict_delete(st);
  scope = comp_stack_pop(scope);
}

void ast_set_type(comp_tree_t *ast, int type) {
  iks_ast_node_value_t *astn = ast->item;
  astn->iks_type = type;
}

>>>>>>> Stashed changes
