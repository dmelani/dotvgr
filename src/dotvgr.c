#include <graphviz/cgraph.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
vagrant_prologue() {
	printf("Vagrant.configure(\"2\") do |config|\n");
}

void
vagrant_host(const char *name) {
	printf("\tconfig.vm.define \"%s\" do |%s|\n", name, name);
	printf("\t\t%s.vm.box = \"debian/wheezy64\"\n", name);
	printf("\tend\n\n");
}

void
vagrant_epilogue() {
	printf("end\n");
}


void
usage(const char *name) {
	printf("Usage:\n\t%s dotfile\n", name);
}

struct topology {
	char *prefix;
	int nhosts;
	int maxhosts;
	struct host *hosts;
	int nlinks;
	int maxlinks;
	struct link *links;
	int nnetworks;
	int maxnetworks;
	struct network *networks;
};

struct network {
	char *name;
	char *prefix;
	int nlinks;
	int maxlinks;
	struct link **links;
};

struct link {
	char *name;
	char *addr;
	struct network *network;
};

struct host {
	char *name;
	int nlinks;
	int maxlinks;
	struct link **links;
};

void
vagrant_render(struct topology *t) {
	vagrant_prologue();
	for (int i = 0; i < t->nhosts; i++)
		vagrant_host(t->hosts[i].name);
	vagrant_epilogue();
}

struct topology *
create_topology(const char *prefix) {
	struct topology *t = malloc(sizeof(*t));

	t->prefix = strdup(prefix);
	
	t->nhosts = 0;
	t->maxhosts = 10;
	t->hosts = calloc(t->maxhosts, sizeof(*t->hosts));

	t->nlinks = 0;
	t->maxlinks = 10;
	t->links = calloc(t->maxlinks, sizeof(*t->links));

	t->nnetworks = 0;
	t->maxnetworks = 10;
	t->networks = calloc(t->maxnetworks, sizeof(*t->networks));

	return t;
}

void
topology_add_host(struct topology *t, char *name) {
	if (t->nhosts == t->maxhosts) {
		t->maxhosts <<= 2;
		t->hosts = realloc(t->hosts, sizeof(*t->hosts) * t->maxhosts);
	}

	struct host *h = &t->hosts[t->nhosts++];
	h->name = name;
}

struct host *
topology_find_host(struct topology *t, const char *name) {
	for (int i = 0; i < t->nhosts; i++) {
		if (strcmp(name, t->hosts[i].name) == 0)
			return &t->hosts[i];
	}

	return NULL;
}

void
topology_connect_hosts(struct topology *t, const char *a, const char *b) {
	struct host *a = topology_find_host(t, a);
	struct host *b = topology_find_host(t, b);

	if (!(a && b))
		return;

	// continue here	
}

int
main(int argc, char **argv) {
	Agraph_t *g;
	FILE *f;
	struct topology *t = create_topology("192.168");

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

	/* create hosts */
	for (Agnode_t *n = agfstnode(g); n; n = agnxtnode(g, n)) {
		topology_add_host(t, strdup(agnameof(n)));
	}

	/* link hosts */
	for (Agnode_t *n = agfstnode(g); n; n = agnxtnode(g, n)) {
		for (Agedge_t *e = agfstedge(g, n); e; e = agnxtedge(g, e, n)) {
			//network_connect_hosts(n, agnameof(n), agnameof(e->node));
		}
	}

	vagrant_render(t);


	return 0;
}

