package org.example;
import java.sql.SQLException;

public class DataServiceException extends RuntimeException {

   public DataServiceException(SQLException e){
      throw new RuntimeException(e);
   }

}
