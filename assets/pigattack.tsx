<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.5" tiledversion="1.5.0" name="pigattack" tilewidth="34" tileheight="28" tilecount="5" columns="5">
 <properties>
  <property name="Facing Left" type="bool" value="true"/>
 </properties>
 <image source="spritesheets/kingsandpigs-10fps/03-Pig/Attack (34x28).png" width="170" height="28"/>
 <tile id="0">
  <objectgroup draworder="index" id="3">
   <object id="2" type="body" x="12" y="12" width="15" height="16"/>
  </objectgroup>
 </tile>
 <tile id="1">
  <objectgroup>
   <object id="1" type="body" x="12" y="12" width="15" height="16"/>
  </objectgroup>
 </tile>
 <tile id="2">
  <objectgroup draworder="index" id="2">
   <object id="1" type="body" x="12" y="12" width="15" height="16"/>
   <object id="2" type="attack" x="1" y="2" width="9" height="20"/>
  </objectgroup>
 </tile>
 <tile id="3">
  <objectgroup draworder="index" id="2">
   <object id="1" type="body" x="12" y="12" width="15" height="16"/>
   <object id="2" type="attack" x="5" y="0" width="8" height="10"/>
  </objectgroup>
 </tile>
 <tile id="4">
  <objectgroup>
   <object id="1" type="body" x="12" y="12" width="15" height="16"/>
  </objectgroup>
 </tile>
</tileset>
