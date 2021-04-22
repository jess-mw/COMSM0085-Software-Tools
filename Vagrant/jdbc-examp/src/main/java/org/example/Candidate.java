package org.example;

public class Candidate {
    public int id;
    public String name;
    public Party party;
    public Ward ward;
    public int votes;

    public Candidate(int id, String name, Party party, Ward ward, int votes){
      this.id = id;
      this.name = name;
      this.party = party;
      this.ward = ward;
      this.votes = votes;
   }
}
