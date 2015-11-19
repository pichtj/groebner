encodeAsString = (I) -> (concatenate between(",", apply(gens ring I, toString)) | "\n" | concatenate between(",", apply(flatten entries gens I, toString)))

moGVW = (I) -> (
    f = openInOut "!./moGVW";
    f << encodeAsString I << closeOut;
    matrix { apply(lines get f, value) }
)
