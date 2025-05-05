JC = javac
JFLAGS = -classpath .
JD = javadoc
JDFLAGS = -protected -splitindex -use -author -version -d ./javadoc
RM = rm
JR = java

CLASSES = \
		AVLTree.java \
		GUIVisualization.java \
		Main.java \
		Stock.java \
		StockDataManager.java \
		RandomInputGenerator.java

all : Main.class

run :
	$(JR) Main random_input.txt

generate : RandomInputGenerator.class
	$(JR) RandomInputGenerator

visualize: GUIVisualization.class
	$(JR) GUIVisualization

classes : $(CLASSES:.java=.class)

%.class : %.java
	$(JC) $(JFLAGS) $<

doc:
	$(JD) $(JDFLAGS) *.java

clean:
	$(RM) -r -f *.class

cleandoc:
	$(RM) -r ./javadoc