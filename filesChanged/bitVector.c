

unsigned char returnSpecificElement(unsigned char * bitVector, int place)
{
        unsigned char charContainingElement = bitVector[place / 8];

        charContainingElement = charContainingElement << place % 8;
        charContainingElement = charContainingElement >> 7;

        return charContainingElement;
}

void changeSpecificElement(unsigned char * bitVector, int place)
{
        bitVector[place / 8] ^= 1 << (7 - place % 8);
        return;
}
