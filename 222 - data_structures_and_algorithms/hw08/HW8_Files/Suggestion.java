public class Suggestion {
	Person person;
	double score;
	int mutualFriends;
	int commonHobbies;

	public Suggestion(Person person, double score, int mutualFriends, int commonHobbies) {
		this.person = person;
		this.score = score;
		this.mutualFriends = mutualFriends;
		this.commonHobbies = commonHobbies;
	}

	public double getScore() {
		return score;
	}
}
