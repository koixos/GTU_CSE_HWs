JC = javac
JFLAGS = -classpath .
JD = javadoc
JDFLAGS = -protected -splitindex -use -author -version -d ./javadoc
RM = rm
JR = java

CLASSES = \
		Main.java \
		Person.java \
		SocialNetworkGraph.java

all : Main.class Person.class SocialNetworkGraph.class

run :
	$(JR) Main

classes : $(CLASSES:.java=.class)

%.class : %.java
	$(JC) $(JFLAGS) $<

doc:
	$(JD) $(JDFLAGS) *.java

clean:
	$(RM) -r -f *.class

cleandoc:
	$(RM) -r ./javadoc