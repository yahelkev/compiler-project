#define _CRT_SECURE_NO_WARNINGS
#include "variable.h"

VARIABLE* variable(char* name, char* value, int type) {
	VARIABLE* new_ = (VARIABLE*)malloc(sizeof(VARIABLE));
	new_->free = &__FREEVARIABLE__;
	new_->str = &__STRVARIABLE__;

	new_->varName = (char*)malloc(sizeof(char) * (1+strlen(name)));
	new_->value = (char*)malloc(sizeof(char) * (1+strlen(name)));

	new_->type = type;
	strcpy(new_->varName, name);
	strcpy(new_->value, value);
	return new_;
}
void __FREEVARIABLE__(VARIABLE* var) {
	free(var->varName);
	free(var->value);
	free(var);
	return;
}
char* __STRVARIABLE__(VARIABLE* var) {
	char* STR_ = (char*)malloc(sizeof(char) * (strlen(var->value) + strlen(var->varName) + 9));
	if (!STR_) return "";
	char typeS[3] = { 0,0,0 };
	_itoa(var->type, typeS, 10);
	STR_[0] = '[';
	STR_[1] = '\0';
	strcat(STR_, typeS);
	strcat(STR_, "] _");
	strcat(STR_, var->varName);
	strcat(STR_, "_ ");
	strcat(STR_, var->value);
	return STR_;
	// type size -> (int)((ceil(log10(num))+1)*sizeof(char))
	// [type] _name_ value
}