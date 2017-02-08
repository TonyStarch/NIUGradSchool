select Name
from Student
where ID NOT IN 
   (select StudentID
   from enrolled);
exit
