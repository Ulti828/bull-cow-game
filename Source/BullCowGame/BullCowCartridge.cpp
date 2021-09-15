// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);
    
    SetupGame(); // Calling the declared Function in the header file
}    
void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{    
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else 
    {
        ProcessGuess(PlayerInput); // checking PlayerGuess
    }
    
}

void UBullCowCartridge::SetupGame() //This shows what its doing below the other functions
{
            //Welcomes player
    PrintLine(TEXT("Welcome to Bullgame!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = 5;
    bGameOver = false;

    PrintLine(TEXT("Please guess the %i letter word!"), HiddenWord.Len()); // Remove this hard coded line for word guess
    //PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); //Debug Line we can turn on and off when testing the game.
    PrintLine(TEXT("You have %i Lives!"), Lives);
    PrintLine(TEXT("Type in your guess.")); //prompts player for guess 
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;

    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) 
    {
        ClearScreen();
        PrintLine(TEXT("You've won!"));
        EndGame();
        return;
    } 
    else if (Guess.Len() != HiddenWord.Len())
    {
        ClearScreen();
        PrintLine(TEXT("The Hidden Word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again!  \n You have %i lives left!"), Lives);  //show lives
        return;
    }
    else if (!IsIsogram(Guess)) //check to make sure its an isogram word
    {
        PrintLine(TEXT("No repeating letters, guess again."));
        return;
    }
    else
    {
        ClearScreen();
        --Lives; 
        PrintLine(TEXT("You've lost a life!"));  //Remove a life from the players
    }
    

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You've run out of lives."));
        PrintLine(TEXT("The Hidden Word was: %s."), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);

    PrintLine(TEXT("%i lives remaining."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{

TArray<FString> ValidWords;

    for (FString Word : WordList) // Ranged based for loop. Formatted string on a Temp. variable to read the TArray WordList
    {   
       if (Word.Len() >= 2 && Word.Len() <= 17 && IsIsogram(Word)) //Checking if word length is greater than 4 AND less than 8
        {
            ValidWords.Emplace(Word);
        }
    } 
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{

    FBullCowCount Count;

    // for every Index guess is the same as Index Hidden, Count.Bulls++
    // If not a bull, was it a cow? If yes, Count.Cows++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for  (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}