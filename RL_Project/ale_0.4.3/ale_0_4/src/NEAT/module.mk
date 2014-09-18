MODULE := src/NEAT

MODULE_OBJS := \
	src/NEAT/gene_node.o \
	src/NEAT/gene_connection.o \
	src/NEAT/genotype.o \
	src/NEAT/species.o \
	src/NEAT/generation.o \
	src/NEAT/innovation.o \
	src/NEAT/experiment.o \

MODULE_DIRS += \
	src/NEAT

# Include common rules 
include $(srcdir)/common.rules
