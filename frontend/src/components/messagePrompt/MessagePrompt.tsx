import { useId } from "react"
import styles from "./MessagePrompt.module.css"

import { Button, Input, Label } from "@/components/UI"
import cls from "@/utils/cls"
import { ReactSVG } from "react-svg"

import { useChatStore } from "@/state/chat"
import MessageService from "@/api/MessageService"
import useInput from "@/hooks/useInput"

interface MessagePromptProps {
    className?: string
}

export const MessagePrompt = ({
    className = ""
}: MessagePromptProps): JSX.Element => {
    const id = useId()
    const chat = useChatStore(state => state.data)
    const addMessages = useChatStore(state => state.addMessage)
    const user = useChatStore(state => state.data)
    const [message,] = useInput("")

    const OnChooseFileClick = (e: React.MouseEvent) => {
        e.preventDefault()
        alert("In development")
    }

    const onSendMessage = async (e: React.FormEvent<HTMLFormElement>) => {
        console.log(chat)
        e.preventDefault()
        if (chat === null) return
        if (user === null) return
        let create_message = {
            chatId: chat.id,
            authorId: user.id,
            body: message.value,
        }
        const data = await MessageService.createOne(create_message)
        if (data === null) {
            alert("Error")
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
                <Button type="submit" variant="send">Send</Button>
            </form>
        </div>
    )
}
