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

all : Main.class GUIVisualization.class Stock.class StockDataManager.class RandomInputGenerator.class

run :
	$(JR) Main random_input.txt

generate :
	$(JR) RandomInputGenerator

classes : $(CLASSES:.java=.class)

%.class : %.java
	$(JC) $(JFLAGS) $<

doc:
	$(JD) $(JDFLAGS) *.java

clean:
	$(RM) -r -f *.class

cleandoc:
	$(RM) -r ./javadoc