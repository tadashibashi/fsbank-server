# Insound Schemas

"Schemas" in Insound are structs and classes intended for JSON
serialization/deserialization and used in Mongo Models and Documents.

Classes and struct names ending -`Token` are intended to be passed to and from
the frontend.

## JSON Serialization & Deserialization
Classes and structs intended for serialization must be registered with
[Glaze](https://github.com/stephenberry/glaze), a JSON library used by Insound,
by implementing the `glz::meta` struct.

This can be done in two ways, locally, and globally as seen in the
[Glaze docs](https://github.com/stephenberry/glaze#struct-registration-macros).

When setting up JSON schemas for `Token` types, please make sure to hide any
sensitive data from the frontend via `glz::hide` so that sensitive data will
not be revealed to the client.

Glaze meta class implementation should be decoupled from the
main class header in a file `<struct_name>.json.h`, where the original file is
`<struct_name>.h`. This enables consumers not interested in JSON serialization
or deserialization to use the struct without needing to consume the Glaze
headers.

## MongoDB Models & Documents
You can create a MongoDB model simply by using a struct that specializes the
`glz::meta` struct as a type parameter in the Model constructor like so:
`auto TypeModel = Insound::Mongo::Document<TypeName>();`

From here you can query the database and create new documents with its member
functions. TODO: Examples should be written here. For now, please check out
the doc comments in files located in the `insound/core/mongo` folder.
