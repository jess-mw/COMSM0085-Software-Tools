package org.example;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.List;
import java.util.ArrayList;

public class DataService {

   private Connection c;
   public static String CS = "jdbc:mariadb://localhost:3306/elections?user=vagrant&localSocket=/var/run/mysqld/mysqld.sock";

   public DataService(String CS, int id){
      c = DriverManager.getConnection(CS);
      List<Party> partyList = getParties();
      System.out.println(partyList);
      Party party = getParty(id);
      System.out.println(party);
      c.close();
   }

   public List<Party> getParties() {
      String SQL = "SELECT * FROM Party ;";
      List<Party> partyList = new ArrayList<Party>();
      try(PreparedStatement s = c.prepareStatement(SQL)){
         ResultSet r = s.executeQuery();
         while (r.next()) {
            int id = r.getInt("id");
            String name = r.getString("name");
            Party party = new Party(id, name);
            partyList.add(party);
         }
      } catch (SQLException e){
         throw new DataServiceException(e);
      }
      return partyList;
   }

   public Party getParty(int id) {
      String SQL = "SELECT id, name FROM Party WHERE " + "id = ? ;";
      try (PreparedStatement s = c.prepareStatement(SQL)) {
          s.setInt(1, id);
          ResultSet r = s.executeQuery();
          if (r.next()) {
             String name = r.getString("name");
             Party party = new Party(id, name);
             return party;
          }
          else {
             return null;
          }
      } catch (SQLException e) {
          throw new DataServiceException(e);
      }
   }

   public static void main(String[] args) {
       int id = Integer.parseInt(args[0]);
       if(args == null){
         throw new RuntimeException("No argument passed to main");
       }
       else{
         DataService dataService = new DataService(CS, id);
       }
   }
}
