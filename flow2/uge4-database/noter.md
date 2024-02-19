# Database

## normalisering

### normalformer

1. **første normalform**: 
  1. Der skal være en nogle der entydigt identificerer 
  en enkelte række i tabellen.
  2. De enkelte felter må kun indeholde en værdi (atomare værdier).
  3. Der må ikke være kolonner der gentages.

2. **anden normalform**:
  1. Opfylder ovenstående
  2. Ingen attributter/egenskaber der ikke selv tilhører nøglen, 
  må afhænge af en del af nøglen.
3. **tredje normalform**: 
  1. Opfylder ovenstående.
  2. Ingen attributter/egenskaber må afhænge af andre attributter, 
  der ikke selv er nøgler.
