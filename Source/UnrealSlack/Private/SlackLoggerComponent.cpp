// Fill out your copyright notice in the Description page of Project Settings.

#include "SlackLoggerComponent.h"
#include "Json.h"
#include "JsonUtilities.h"


USlackLoggerComponent::USlackLoggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USlackLoggerComponent::BeginPlay()
{
	Super::BeginPlay();
	Http = &FHttpModule::Get();
}

void USlackLoggerComponent::Post(const FString& SlackMessage)
{
	TSharedRef<IHttpRequest> Request = PostRequest(MakeSlackFormatMessage(SlackMessage));
	Request->OnProcessRequestComplete().BindUObject(this, &USlackLoggerComponent::SlackResponse);
	Request->ProcessRequest();
}

TSharedRef<IHttpRequest> USlackLoggerComponent::PostRequest(const FString& ContentJsonString)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetHeader(TEXT("Content-Length"), *FString::FromInt(ContentJsonString.Len()));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetURL(SlackUrl);	
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);

	return Request;
}

void USlackLoggerComponent::SlackResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Smth goes wrong"));
	}
	else if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response->GetContentAsString());
	}	
}

FString USlackLoggerComponent::MakeSlackFormatMessage(const FString& SlackMessage) const
{
	return "{\"text\": \"" + SlackMessage + "\"}";
}

