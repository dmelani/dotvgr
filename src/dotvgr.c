#include <graphviz/cgraph.h>

#include <stdio.h>

void
vagrant_prologue() {
	printf("Vagrant.configure(\"2\") do |config|\n");
}

void
vagrant_host(const char *name) {
	printf("\tconfig.vm.define \"%s\" do |%s|\n", name, name);
	printf("\tend\n");
}

void
vagrant_epilogue() {
	printf("end\n");
}

void
usage(const char *name) {
	printf("Usage:\n\t%s dotfile\n", name);
}

int
main(int argc, char **argv) {
	Agraph_t *g;
	FILE *f;

	if (argc != 2) {
		usage(argv[0]);
		return -1;
	}

	if ((f = fopen(argv[1], "r")) == NULL) {
		perror("Failed to open dotfile");
		return -1;
	}

	if ((g = agread(f, NIL(Agdisc_t *))) == NULL) {
		printf("Failed to parse %s\n", argv[1]);
	}

	vagrant_prologue();
	for (Agnode_t *n = agfstnode(g); n; n = agnxtnode(g, n)) {
		vagrant_host(agnameof(n));
	}
	vagrant_epilogue();

	return 0;
}

