// Copyright by Free2Play-Entertainment (2020)


#include "Login.h"

ULogin* ULogin::Login(const FString UserName, const FString UserToken)
{
    ULogin* LoginNode = NewObject<ULogin>();
    LoginNode->Name = UserName;
    LoginNode->Token = UserToken;
    return LoginNode;
}

void ULogin::Activate()
{
    if(!Super::Validate())
    {
        Failure.Broadcast();
        return;
    }
    FScriptDelegate funcDelegate;
    funcDelegate.BindUFunction(this, "Callback");
    FieldData = UJsonData::GetRequest(UGameJolt::CreateURL("users/auth/?username=" + Name + "&user_token=" + Token));
    FieldData->OnGetResult.AddUnique(funcDelegate);
}

void ULogin::Callback(const bool bSuccess, UJsonData* JSON)
{
    Super::Callback(bSuccess, JSON);
    if(!bResponseValid)
    {
        Failure.Broadcast();
        return;
    }

    UGameJolt::Get()->Login(Name, Token);
    Success.Broadcast();
}