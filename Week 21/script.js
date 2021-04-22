const tableHeaderHTML = `
<tr>
   <th>Name</th>
</tr>
`;

const petRowHTML = `
<tr>
   <td>PET_NAME</td>
</tr>
`;

const baseUrl = 'https://petstore.swagger.io/v2';
const resourcePath = '/pet/findByStatus';
const query = '?status=available';

let petsHTML = tableHeaderHTML;

function display()
{
   document.getElementById('yourPetsDisplayName').innerHTML = document.getElementById('petName').value;
   document.getElementById('petName').value = '';
   document.getElementById('yourPetsDisplaySpecies').innerHTML = document.getElementById('species').value;
   document.getElementById('species').value = '';
}

function addRow()
{
   var petName = document.getElementById('petName').value;
   var species = document.getElementById('species').value;
   var parent1=document.createElement('tr');
   var parent2=document.createElement('tr');
   var col1=document.createElement('td');
   var col2=document.createElement('td');
   var text1=document.createTextNode(petName);
   var text2=document.createTextNode(species);
   col1.appendChild(text1);
   col2.appendChild(text2);
   parent1.appendChild(col1);
   parent2.appendChild(col2);
   document.getElementById('yourPetsDisplayName').appendChild(parent1);
   document.getElementById('yourPetsDisplaySpecies').appendChild(parent2);
   document.getElementById('petName').value="";
   document.getElementById('species').value="";
}

function handleClick(){
   const path = `${baseUrl}${resourcePath}${query}`;
   fetch(path)
      .then(response=>response.json())
      .then(data => {
         if(!Array.isArray(data)){
            throw new Error('The data is invalid');
         }
         data.forEach(pet => {
            petsHTML+=petRowHTML.replace('PET_NAME', pet.name);
         });

   document.getElementById('petsTable').innerHTML = petsHTML;
   })
   .catch((error)=>{
      console.log('ERROR HAS OCCURED: ', JSON.stringify(error))
   });
}
