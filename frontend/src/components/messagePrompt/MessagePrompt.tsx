import { useId } from "react"
import styles from "./MessagePrompt.module.css"

import { Button, Input, Label } from "@/components/UI"
import cls from "@/utils/cls"
import { ReactSVG } from "react-svg"

import { useChatStore } from "@/state/chat"
import { useUserStore } from "@/state/user"
import MessageService from "@/api/MessageService"
import useInput from "@/hooks/useInput"
import { IChat } from "@/types"

interface MessagePromptProps {
    className?: string
}

export const MessagePrompt = ({
    className = ""
}: MessagePromptProps): JSX.Element => {
    const id = useId()
    const chat = useChatStore(state => state.data)
    const addMessages = useChatStore(state => state.addMessage)
    const setCurrentChats = useChatStore(state => state.setCurrentChat)
    const currentChat = useChatStore(state => state.data)
    const user = useUserStore(state => state.data)
    const [message, setMessage] = useInput("")

    const OnChooseFileClick = (e: React.MouseEvent) => {
        e.preventDefault()
        alert("In development")
    }

    const onSendMessage = async (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault()
        if (chat === null) {
            console.error("chat == null")
            return
        }
        if (user === null) {
            console.error("user == null")
            return
        }
        let create_message = {
            chatId: chat.id,
            authorId: user.id,
            body: message.value,
        }
        setMessage("")
        const data = await MessageService.createOne(create_message)
        setCurrentChats({ ...currentChat, lastMessage: data } as IChat)
        if (data === null) {
            alert("Error on create message")
            return
        }
        addMessages(data)
    }

    return (
        <div className={cls(styles.message_prompt, className)}>
            <form action="" onSubmit={onSendMessage}>
                <Label htmlFor={id} variant="file" onClick={OnChooseFileClick}>
                    <ReactSVG
                        className={styles.icon}
                        src="/file.svg"
                    />
                    <Input id={id} type="file" variant="file" />
                </Label>
                <Input type="text" variant="message_input" placeholder="Message..." className={styles.prompt} {...message} />
                <Button type="submit" variant="send" disabled={message.value === ""}>Send</Button>
            </form>
        </div>
    )
}
